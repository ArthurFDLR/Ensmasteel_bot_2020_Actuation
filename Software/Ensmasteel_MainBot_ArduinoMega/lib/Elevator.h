#ifndef ELEVATOR_INCLUDED
#define ELEVATOR_INCLUDED
#include "Arduino.h"
#include "Moteur.h"
#include "1_CONSTANTS.h"

class Contacteur;
class Codeuse;
class PID;

class Elevator
{
  public:
	  Motor* moteurElevator;
	  ElevatorStateE state;
	  float aim;
	  Contacteur *contacteurElevator;
	  Codeuse *codeuseElevator;
	  PID *pidElevator;



	  Elevator(uint8_t pinContacteur,uint8_t pin1Codeuse,uint8_t pin2Codeuse,float tickToPos,uint8_t pinMoteurPwr,uint8_t pinMoteurSens,uint8_t pinMoteurBrake);
	  Elevator();
	  void actuate(float dt);
	  bool goodenough();

	  bool init();
};

#endif
