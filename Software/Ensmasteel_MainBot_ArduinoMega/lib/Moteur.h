#ifndef MOTEUR_INCLUDED
#define MOTEUR_INCLUDED

#include "1_CONSTANTS.h"
#include "Arduino.h"

class Motor
{
public:
    uint8_t pinPWR,pinSens,pinBrake;   //Pin de puissance (PWM) et les deux pin pour donner le sens de rotation
    int32_t order,masterOrder;              //Le regime moteur entre -65535 et 65535
    bool bypass=false;
    Motor();
    Motor(uint8_t in_pinPWR, uint8_t in_pinSens, uint8_t in_pinBrake);
    void actuate();                     //Actualisation de la puissance moteur
};

#endif
