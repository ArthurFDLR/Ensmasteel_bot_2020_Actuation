#include "Elevator.h"
#include "Contacteur.h"
#include "Codeuse.h"
#include "PID.h"
#include "Moteur.h"
Elevator::Elevator(uint8_t pinContacteur,uint8_t pin1Codeuse,uint8_t pin2Codeuse,float tickToPos,uint8_t pinMoteurPwr,uint8_t pinMoteurSens,uint8_t pinMoteurBrake)
{
  moteurElevator=new Motor(pinMoteurPwr,pinMoteurSens,pinMoteurBrake);
  contacteurElevator=new Contacteur(pinContacteur);
  codeuseElevator=new Codeuse(false,pin1Codeuse,pin2Codeuse,tickToPos);
  aim=0;
  pidElevator=new PID(false, 50000.0, 0, 1000.0, 50, 0);
}

Elevator::Elevator()
{

}

bool Elevator::goodenough()
{
    return abs(codeuseElevator->pos-aim)<0.005  && abs(codeuseElevator->dPos)<0.005;
}


void Elevator::actuate(float dt)
{
    codeuseElevator->actuate(dt);
    moteurElevator->order=pidElevator->compute(dt,aim,codeuseElevator->pos,codeuseElevator->dPos);
    moteurElevator->actuate();
    //Serial.print("posElev ");Serial.println(codeuseElevator->pos);
}


bool Elevator::init()
{
  float t=millis()/1000.0;
  float lastT=millis()/1000.0 - 0.01;
  float tIni1=t;
  while (!contacteurElevator->isPressed() and t-tIni1<20)
  {
    t=millis()/1000.0;
    float dt=t-lastT;
    lastT=t;
    moteurElevator->order=-50;   //On fait descendre l'elevator doucement
    moteurElevator->actuate();
    codeuseElevator->actuate(dt);
  }

  moteurElevator->order=0;// On arrete l'elevateur
  moteurElevator->actuate();

  if (t-tIni1>=20)return false;
  else
  {
    codeuseElevator->reset();
    aim=AIMAboveBarel;
    //aim=AIMDistribLevel;
    return true;
  }

}
