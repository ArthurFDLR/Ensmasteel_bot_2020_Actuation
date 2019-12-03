#ifndef FILTRE_INCLUDED
#define FILTRE_INCLUDED
#include "Arduino.h"
#include "1_CONSTANTS.h"

class Filtre
{                                
	public:
		void in(float newRaw,float dt);               //Entrée d'une donnée brute
		float out();                                  //Sortie d'une donnée filtrée
		Filtre(float initValue,float frequency,typeFiltreE typeFiltre);
    void reset(float initValue);
		Filtre();

	private:
		float lastValue, lastLastValue, raw, lastRaw; //Frequence de coupure
		typeFiltreE typeFiltre;                     //Ordre du filtre
		float Wc;
};

#endif
