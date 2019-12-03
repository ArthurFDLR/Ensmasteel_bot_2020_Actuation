#include "Filtre.h"
#include "Arduino.h"
void Filtre::reset(float initValue)
{
  lastValue=initValue;
  lastLastValue=initValue;
  raw=initValue;
  lastRaw=initValue;
}

void Filtre::in(float newRaw,float dt)
{
    //ON stock la valeur brute au cas ou
    raw=newRaw;
    if (dt>0.0000001)
    {
      switch (typeFiltre)
      {
        case PAS_DE_FILTRE: {lastValue=newRaw;break;} //A l'ordre 0, on ne fait rien
        case LOWPASS1: {lastValue=(lastValue+newRaw*dt*Wc)/(Wc*dt+1);break;}
        case LOWPASS2:
          {
            float alpha=Wc*dt;
            float out=( alpha*newRaw + 2*lastValue*(1+1/alpha) - lastLastValue/alpha ) / (alpha + 2 + 1/alpha);
            lastLastValue=lastValue;
            lastValue=out;
            break;
          }
        case DELAY: //L'ordre 3 est un retard
        {
          lastValue=(newRaw - Wc*(newRaw - lastRaw - lastValue)/dt ) / (1 + Wc/dt);
          lastRaw=newRaw;
        }
      }
    }
}

float Filtre::out()
{
  return lastValue;
}

Filtre::Filtre(float initValue,float frequency,typeFiltreE typeFiltre)
{
  this->lastValue=initValue;
  this->lastLastValue=initValue;
  this->lastRaw=initValue;
  this->raw=initValue;
  this->typeFiltre=typeFiltre;
  (typeFiltre=DELAY)?(this->Wc=frequency/2.0):(this->Wc=2*PI*frequency);
}

Filtre::Filtre()
{

}
