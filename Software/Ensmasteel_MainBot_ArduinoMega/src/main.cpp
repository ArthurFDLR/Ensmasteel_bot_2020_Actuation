#include <Arduino.h>
#include "StateMachine.h"
#include "Logger.h"
#include "Communication.h"
#include "Mega.h"

StateMachine sm;
Communication com;

float dt=0.1;

class Pince
{
private:
  StateMachine* stateMachine;
public:
  Pince(StateMachine* stateMachine) {this->stateMachine = stateMachine;}
  Pince(){}

  void grab()
  {
    delay(2000);
    if ((millis()/100)%2==0) //(random du feignant)
      stateMachine->gripConfirmed();
  }
};

Pince pince;

class WaitForTirette : public State
{
protected:
  uint8_t pin;
public:
  WaitForTirette(float timeout, uint8_t pin) : State(timeout){this->pin = pin;}

  void onStart() override
  {
    pinMode(pin,INPUT_PULLUP);
  }

  void onRun(float dt) override
  {
    if (digitalRead(pin)==HIGH)
      stateMachine->changeCurrentState(getStateStd());
  }

  void onLeave() override
  {
    stateMachine->com->send(newMessage(MessageID::Tirette_M));
  }
};

class PinceGrab : public State
{
private:
  Pince* pince;

public:
  PinceGrab(float timeout, Pince* pince) : State(timeout){this->pince = pince;}

  void onStart() override{
    pince->grab();
  }

  void onGripConfirmed() override{
    stateMachine->changeCurrentState(getStateStd());
  }

  void onTimeout() override{
    stateMachine->changeCurrentState(getStateErr());
  }
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Logger::setup(&Serial,&Serial,&Serial,false,true,true);
  com = Communication(&Serial);
  sm = StateMachine(&com);
  pince = Pince(&sm);
  
  State* waitTirette = new WaitForTirette(-1,PB4);
  sm.addState(waitTirette);
  
  State* pinceGrab = new PinceGrab(10,&pince);
  sm.addState(pinceGrab);
  waitTirette->setStateStd(pinceGrab);

  State* laSuiteQuiNestPasCodee = new State(-1);
  sm.addState(laSuiteQuiNestPasCodee);
  pinceGrab->setStateStd(laSuiteQuiNestPasCodee);
  
  State* failure = new State(-1);
  sm.addState(failure);
  pinceGrab->setStateErr(failure);
  
  Logger::infoln("START");
  sm.setStartState(waitTirette);
}

void loop() {
  Logger::setup(&Serial,&Serial,&Serial,true,true,true);
  com.update();
  if (com.inWaitingRx()>0)
    com.popOldestMessage();
  
  sm.update(dt);
  delay(50);
}