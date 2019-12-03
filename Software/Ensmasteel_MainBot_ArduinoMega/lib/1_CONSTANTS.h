#ifndef CONSTANTS
#define CONSTANTS
#define ACCBARILLETSTD 3.0

#define STATE
//---------------------------------- PINS&Servo   --------------------------------


#define BRAS_GAUCHE_PIN 3
#define BRAS_GAUCHE_RETRACTED 2300          //Vertical
#define BRAS_GAUCHE_HALF_RETRACTED 1500     //OSEF
#define BRAS_GAUCHE_HALF_EXTENDED 1450      //OSEF
#define BRAS_GAUCHE_EXTENDED 1330           //Horizontal  //1330

#define BRAS_DROIT_PIN 2
#define BRAS_DROIT_RETRACTED 1230           //Vertical
#define BRAS_DROIT_HALF_RETRACTED 1500      //Osef
#define BRAS_DROIT_HALF_EXTENDED 2120       //OSEF
#define BRAS_DROIT_EXTENDED 2250             //Horizontal //2250

#define DOIGT_GAUCHE_PIN 5
#define DOIGT_GAUCHE_RETRACTED 1190         //Bloquage palet
#define DOIGT_GAUCHE_HALF_RETRACTED 1500    //Osef
#define DOIGT_GAUCHE_HALF_EXTENDED 1500     //Osef
#define DOIGT_GAUCHE_EXTENDED 1700          //Caché

#define DOIGT_DROIT_PIN 4
#define DOIGT_DROIT_RETRACTED 1790           //Bloquage palet
#define DOIGT_DROIT_HALF_RETRACTED 1500     //OSEF
#define DOIGT_DROIT_HALF_EXTENDED 1500      //OSEF
#define DOIGT_DROIT_EXTENDED  1280         //caché

#define PINCE_GAUCHE_PIN 7
#define PINCE_GAUCHE_RETRACTED 2270         //pince cachée
#define PINCE_GAUCHE_HALF_RETRACTED 1550    //Capture palet
#define PINCE_GAUCHE_HALF_EXTENDED 950    //Entonnoire
#define PINCE_GAUCHE_EXTENDED 650           //Depart

#define PINCE_DROITE_PIN 6
#define PINCE_DROITE_RETRACTED 780          //Pince cachée
#define PINCE_DROITE_HALF_RETRACTED 1340    //Capture palet
#define PINCE_DROITE_HALF_EXTENDED 1950     //Entonnoire
#define PINCE_DROITE_EXTENDED 2300          //Depart

#define MILLIS_CLEAR 250

#define POMPE_GAUCHE_PIN_MOTEUR_PWR     13
#define POMPE_GAUCHE_PIN_MOTEUR_SENS    51
#define POMPE_GAUCHE_PIN_MOTEUR_BRAKE   49
#define POMPE_GAUCHE_PIN_AMP            A0

#define POMPE_DROITE_PIN_MOTEUR_PWR     10
#define POMPE_DROITE_PIN_MOTEUR_SENS    47
#define POMPE_DROITE_PIN_MOTEUR_BRAKE   45
#define POMPE_DROITE_PIN_AMP            A1

#define SHARP_PALET_GAUCHE_PIN          A7
#define SHARP_PALET_DROITE_PIN          A2
#define SHARP_PALET_SEUIL               300

#define SHARP_AVG_PIN                   A4
#define SHARP_AVD_PIN                   A6
#define SHARP_ARG_PIN                   A8
#define SHARP_ARD_PIN                   A3
#define SHARP_ANTICOL_AR_SEUIL          550
#define SHARP_ANTICOL_AV_SEUIL_PROXIMITY     200
#define SHARP_ANTICOL_AV_SEUIL_ALERT    270

#define AMPEREMETRE_POMPE_Droit_PIN         A0
#define AMPEREMETRE_POMPE_Droit_SEUIL_DETECTED   540.
#define AMPEREMETRE_POMPE_Droit_SEUIL_PROXIMITY     500.//osef

#define AMPEREMETRE_POMPE_Gauche_PIN        A1
#define AMPEREMETRE_POMPE_Gauche_SEUIL_DETECTED   530.//563
#define AMPEREMETRE_POMPE_Gauche_SEUIL_PROXIMITY     500.//osef

#define PIN_TIRETTE 27

// --------------------------------- Barillet ---------------------------------

#define BARILLET_PIN_CONTACTEUR       22
#define BARILLET_PIN_CODEUSE_A        24
#define BARILLET_PIN_CODEUSE_B        19
#define BARILLET_PIN_MOTEUR_PWR       8
#define BARILLET_PIN_MOTEUR_SENS      43
#define BARILLET_PIN_MOTEUR_BRAKE     41
#define BARILLET_TickToPos            668.5
#define BARILLET_AngleToInit          1.14 // à determiner exp
#define BARILLET_AngleToNext          1.047975512

enum PaletE
{
  Rouge, Bleu, Vert, VIDE
};

enum AnticolE{Front,Back,No};

enum typeFiltreE {PAS_DE_FILTRE,LOWPASS1,LOWPASS2,DELAY};


// --------------------------------- Elevator ---------------------------------

#define ELEVATOR_PIN_CONTACTEUR       23
#define ELEVATOR_PIN_CODEUSE_A        26
#define ELEVATOR_PIN_CODEUSE_B        18
#define ELEVATOR_PIN_MOTEUR_PWR       9
#define ELEVATOR_PIN_MOTEUR_SENS      35
#define ELEVATOR_PIN_MOTEUR_BRAKE     33
#define ELEVATOR_TickToPos            40000 //80000
#define AIMReadyToTakeOnFloor         0.03 // a determiner exp
#define AIMTakeOnFloor                0.0055
#define AIMAboveFinger                0.14
#define AIMDepositOneFloor            0.08
#define AIMDepositeTwoFloor           0.107
#define AIMDepositThreeFloor          0.13
#define AIMAboveBarel                 0.155
#define AIMDistribLevel               0.02
#define AIMTakeOneFloor               0.0775
#define AIMTakeTwoFloor               0.1025
#define AIMTakeThreeFloor             0.1275
#define AIMAboveAccelerator           0.12
#define AIMInAccelerator              0.08
#define AIMBlueiumAcceleratorLevel    0.055
#define AIMGoldoniumLevel             0.08
#define AIMBalanceLevel               0.095
#define LONGUEUR_COURSE               0.16

enum ElevatorStateE
{
  Moving, ReadyToTakeOnFloor, TakeOnFloor, AboveFinger, DepositOneFloor, DepositeTwoFloor,DepositThreeFloor, AboveBarel, DistribLevel,
  TakeOneFloor,TakeTwoFloor,TakeThreeFloor, AboveAccelerator, InAccelerator, BlueiumAcceleratorLevel, GoldoniumLevel, BalanceLevel
,DeposePaletSol};

// --------------------------------- Messages ---------------------------------

#define NB_MESSAGES 27
enum MessageE
{
    Default,Impossible, Tirette, Pince_Retracted,Pince_Half_Retracted,Pince_Half_Extended,Pince_Extended, Evitemment, Ok, Done,
    New_Action, Sync, Evitemment_Clear,Calle,Violet,Jaune,Start_Chaos,Start_Goldonium,Depose_Goldonium,PoussePaletBleuAccel,DeposePaletSolM
    ,IdleM,VideDistributeurM,Em_stop,DeposeAccelerateurM,DeposePaletBleuBalance}
;


#define BAS 0
#define MILLIEU 1
#define HAUT 2

#define LOWPRIORITY 0
#define MIDPRIORITY 1
#define HIGHPRIORITY 2

#define MAXPWM 255
// --------------------------------- Bras ---------------------------------

#endif // !CONSTANTS
