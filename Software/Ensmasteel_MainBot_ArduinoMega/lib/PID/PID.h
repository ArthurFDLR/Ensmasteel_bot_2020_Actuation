#ifndef PID_H_
#define PID_H_
#include "MoveProfile.h"
#define NBPROFILES ((int)Pace::NB_PACE)
#define TIMETOOFAR 0.2 //Temps qu'il faut rester trop loin pour etre considere tooFar
#include "Filtre.h"
#include "Vector.h"


// La variable x définit une grandeur quelconque. dx est sa derivee.

class PID
{
private:
    MoveProfile* currentProfile; //C'est u pointeur car on veut etre sur qu'il n'y a qu'une seule version d'un profile (pas de copies !)
    float iTerm;
    bool modulo360;   //Permet de dire si les valeurs sont a interprété modulo 360
    Filtre dxF;       //Filtre de la derivee
    bool close;  //Est ce qu'on est proche de la target (cf epsilon et depsilon)
    float lastOut;

public:
    bool isClose(){return close;}
    void reset();
    void setCurrentProfile(MoveProfileName pace);
    float compute(float xTarget, float dxTarget, float x, float dx, float dt); //Renvoie un ordre entre -1 et 1
    MoveProfile* getCurrentProfile();
    PID(bool modulo360, float frequency);
    PID();

};
#endif