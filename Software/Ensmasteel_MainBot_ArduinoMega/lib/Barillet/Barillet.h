#ifndef BARILLET_INCLUDED
#define BARILLET_INCLUDED
#include "PID.h"
#include "Codeuse.h"
#include "1_CONSTANTS.h"
#include "Contacteur.h"
#include "Moteur.h"
#include "Filtre.h"
#include "Color.h"
#include "Math_functions.h"
class Mega;

/**
 * 
 */
class Barillet
{
public:

    Barillet(uint8_t pinContacteur,uint8_t pin1Codeuse,uint8_t pin2Codeuse,float tickToPos,uint8_t pinMoteurPwr,uint8_t pinMoteurSens,uint8_t pinMoteurBrake,Mega* ptrMega);
    Barillet();
    void init();
    bool RedefinitionPosBleuium();
    void actuate(float dt);
    void goTo(float angle);
    void goToDelta(float angle);
    void goToCell(int cell);
    float pos,dPos;
    bool goodenough();
    float posCell(int cell);

private:
    float endTarget,currentAim,currentDAim;
    float tStartMove;
    Trapezoidal_Function trapez;
    Contacteur contacteurBarillet;
    Codeuse codeuseBarillet;
    PID pidBarillet;
    Motor moteurBarillet;
    Color color;
    Mega* ptrMega ;
    float accBarillet=ACCBARILLETSTD;
};

#endif
