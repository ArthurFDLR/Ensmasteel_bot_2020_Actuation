#include <Arduino.h>
#include "StateMachine.h"
#include "Logger.h"
#include "Communication.h"

StateMachine sm;
Communication com;
float dt=0.1;

class WaitTirette : public State
{
public:
  WaitTirette() : State(WaitTiretteN , true) 
  {
    addTransitionSignal(Signal::TiretteS,Sleep10N);
  }

  void onStart() override {Logger::debugln("Waiting for tirette");}

  void onRun(float dt) override
  {
    if (digitalRead(PB4)==HIGH)
      SignalManager::emit(TiretteS);
  }

  void onLeave() override
  {
    com.send(newMessage(Tirette_M));
  }
};




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PB4,INPUT_PULLUP);
  Logger::setup(&Serial,&Serial,&Serial,false,true,true);
  SignalManager::setup();
  com = Communication(&Serial);
  
  sm = StateMachine(&com);
  sm.setState(new WaitTirette());
  sm.setState(new SimpleDelay(Sleep10N,WaitTiretteN,10,false));
  Logger::infoln("START");
}

void loop() {
  com.update();
  sm.update(dt);
  if (com.inWaitingRx()>0)
    com.popOldestMessage();
  SignalManager::clean(1);
}