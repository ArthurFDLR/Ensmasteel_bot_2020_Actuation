#include "Pompe.h"

Pompe::Pompe(uint8_t pinMoteurPwr, uint8_t pinMoteurSens, uint8_t pinMoteurBrake, uint8_t pinAmp,bool inverted)
{
    moteur=new Motor(pinMoteurPwr,pinMoteurSens,pinMoteurBrake);
    pinMode(pinAmp,INPUT);
    if (inverted)
      signe=-1.0;
    else
      signe=1.0;
}

Pompe::Pompe()
{

}

void Pompe::suck()
{
    moteur->order=signe*130;
    moteur->actuate();
}

void Pompe::suckHard()
{
    moteur->order=signe*255;
    moteur->actuate();
}

void Pompe::blow()
{
    moteur->order=-1*signe*215;
    moteur->actuate();
}

void Pompe::stop()
{
    moteur->order=0;
    moteur->actuate();
}

bool Pompe::isSucked()
{
    Serial.print(analogRead(pinAmp));
}
