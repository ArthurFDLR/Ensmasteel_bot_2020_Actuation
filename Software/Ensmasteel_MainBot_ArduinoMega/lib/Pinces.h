#ifndef PINCES_INCLUDED
#define PINCES_INCLUDED
#include "MegaServo.h"
class Pinces
{
    public:
    MegaServo pinceGauche,pinceDroite;
    ServoPosition ordresGauche,ordresDroit;
    int32_t millisNextActionGauche,millisNextActionDroite;
    Pinces(bool dummy);
    Pinces();
    void actuate();
    void pinceGaucheSet(ServoPosition pos,bool wait);
    void pinceDroiteSet(ServoPosition pos,bool wait);
    void bothPincesSet(ServoPosition pos);
};
#endif
