#include "Sharp.h"

Sharp::Sharp()
{
    //ctor
}

void Sharp::actuate(float dt)
{
    this->filtre.in((float)analogRead(pin),dt);
   this->rawVal=filtre.out();
//     this->rawVal=analogRead(pin);
  //  Serial.println(rawVal);
}


Sharp::Sharp(uint8_t pin, float seuil_alerte, float seuil_proximity)
{
    this->pin=pin;
    pinMode(pin,INPUT);
    this->seuil_alerte=seuil_alerte;
    this->seuil_proximity=seuil_proximity;
    this->lastState=SharpState::Normal;
    this->filtre=Filtre(0,30,typeFiltreE::LOWPASS2);
    this->rawVal=0;
}

#define HYSTERESIS 1
SharpState Sharp::getState()
{
    if (rawVal>seuil_alerte)           //Pas d'hyste montante
    {
        lastState=SharpState::Alerte;
        return SharpState::Alerte;
    }
    else if(rawVal<seuil_alerte-HYSTERESIS && lastState==SharpState::Alerte) //Hyste descente
    {
        lastState=SharpState::Proximity;
        return SharpState::Proximity;
    }
    else if(rawVal>seuil_proximity && lastState==SharpState::Normal) //Pas d'hyste montante
    {
        lastState=SharpState::Proximity;
        return SharpState::Proximity;
    }
    else if(rawVal<seuil_proximity-HYSTERESIS && lastState==SharpState::Proximity) //Hyste descente
    {
        lastState=SharpState::Normal;
        return SharpState::Normal;
    }
    else
        return lastState;
}

float Sharp::raw()
{
    return rawVal;
}
