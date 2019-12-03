#ifndef SHARP_INCLUDED
#define SHARP_INCLUDED
#include "Arduino.h"
#include "Filtre.h"
enum SharpState {Normal,Proximity,Alerte};
class Sharp
{
    public:
        Sharp();
        Sharp(uint8_t pin,float seuil_alerte,float seuil_proximity);
        SharpState getState();
        void actuate(float dt);
        float raw();
    private:
        SharpState lastState;
        uint8_t pin;
        float seuil_alerte,seuil_proximity;
        float rawVal;
        Filtre filtre;
};

#endif // SHARP_INCLUDED
