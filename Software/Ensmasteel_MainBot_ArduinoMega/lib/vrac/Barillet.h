#ifndef BARILLET_INCLUDED
#define BARILLET_INCLUDED
#include "PID.h"
#include "Codeuse.h"
#include "1_CONSTANTS.h"
#include "Contacteur.h"
#include "Moteur.h"
#include "Filtre.h"
#include "Color.h"
class Mega;

class Cellule
{
public:
    Cellule(PaletE bas,PaletE millieu,PaletE haut,PaletE highPriority,PaletE midPriority,PaletE lowPriority,float angleDroite,float angleGauche);
    Cellule();
    PaletE stock[3];
    uint8_t nbPaletIn;
    PaletE priorityIn[3];
    float angleGauche,angleDroite;
    void add(PaletE palet);
    void take();
};

class Barillet
{
public:

    Barillet(uint8_t pinContacteur,uint8_t pin1Codeuse,uint8_t pin2Codeuse,float tickToPos,uint8_t pinMoteurPwr,uint8_t pinMoteurSens,uint8_t pinMoteurBrake,bool coteviolet ,Mega* ptrMega);
    Barillet();
    Contacteur *contacteurBarillet;
    Codeuse *codeuseBarillet;
    PID *pidBarillet;
    Motor *moteurBarillet;
    Cellule* cellule[6];
    Color* color;
    Mega* ptrMega ;
    float accBarillet=ACCBARILLETSTD;
    bool init();
    bool RedefinitionPosBleuium();
  //  void addPalet(int cellId, PaletE couleur);
    float Poscellule1,Poscellule2,Poscellule3,Poscellule4,Poscellule5,Poscellule6;
    void actuate(float dT);
    void goTo(float angle);
    void goToDelta(float angle);
    bool goodenough();
    bool coteviolet;
    float pos,dPos;
    private:
    float target,tStartGoto,tFinGoto,tInversion,aim,dAim;
};

#endif
