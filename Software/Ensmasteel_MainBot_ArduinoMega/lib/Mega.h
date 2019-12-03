#ifndef MEGA_INCLUDED
#define MEGA_INCLUDED
//#include "Barillet.h"
#include "Pompe.h"
//#include "Elevator.h"
#include "MegaServo.h"
#include "1_CONSTANTS.h"
#include "Pinces.h"
#include "Sharp.h"
#include "Contacteur.h"
#include "Elevator.h"
#include "Barillet.h"
#include "Comm.h"
enum actionCouranteE {Idle,Chaos,Distrib,RecupGoldonium,DeposeGoldonium,PoussePaletBleu,DeposePaletSolA,VideDistributeur,DeposePaletAccelerateur,DeposeBleuBalance};
enum etapeChaosE {PrepChaos,DescentSouffletSol,RemontePalet,DeposeOneFloor,DeposeRemonte,TourneBarillet};
enum etapeRecupGoldoniumE {Safety,Recup,ElevateurniveauGoldo};
enum etapePaletBleuE {SafetyPal,Pousse};
enum etapeDeposePaletSolE {SafetyEtapeDeposePaletSol,PretADeposeSol,AttrapePaletBarillet,RemontePaletPourDepose,TourneBarilletPourDeposeSol};
enum etapeVideDistributeurE {SafetyEtapeVideDistributeur,PlacementBarilletDistributeur,RecupDistributeur,RemonteVideDistributeur,DescentVideDistributeur};
enum etapeDeposeAccelerateurE{SafetyDeposePaletAccelerateur, AttrapePaletBarilletAccelerateur, RemontePaletPourDeposeAccelerateur,
PretADeposePaletAccelerateur,DescentElevateurDeposePaletAccelerateur,PompeOffDeposePaletAccelerateur,RemonteElevateurPourBarriletDeposePaletAccelerateur,TourneBarilletPourDeposeAccelerateur};
enum etapeDeposeBleuBalanceE {SafetyetapeDeposeBleuBalance,SetBarilletDeposeBleu,DescentElevateurDeposeBleu,RemonteElevateurPourDeposeBalance,JettePaletBalance};
class Mega
{
public:
    Barillet barillet;
    Pompe pompeG, pompeD;
    Elevator elevator;
    MegaServo brasGauche,brasDroit,doigtGauche,doigtDroit;
    Pinces pinces;
    Sharp sharpAVG,sharpAVD,sharpARG,sharpARD,sharpPaletG,sharpPaletD,AmperemetrePompeGauche,AmperemetrePompeDroit;
    Contacteur tirette;
    Comm comm;

    actionCouranteE actionCourante;

    etapeChaosE etapeChaos;
    etapeRecupGoldoniumE etapeRecupGoldonium;
    etapePaletBleuE etapePaletBleu;
    etapeDeposePaletSolE etapeDeposePaletSol;
    etapeVideDistributeurE etapeVideDistributeur;
    etapeDeposeAccelerateurE etapeDeposePaletAccelerateur;
    etapeDeposeBleuBalanceE etapeDeposeBleuBalance;


    int iPosBarillet=1;
    int iPosBarilletVideDistributeur=0;
    int iPosBarilletDeposePaletAccelerateur=0;
    int iPosDeposeBleuBalance=3;
    
    bool sent=false;
    uint32_t millisInit,millisActu;

    void actuate();
    void init();

    bool coteviolet = false ;
    void startTimer();
    bool timerDelay(float delta);

private:
    bool evitting;
    float tTimerStart;


};

#endif // !MEGA_INCLUDED
