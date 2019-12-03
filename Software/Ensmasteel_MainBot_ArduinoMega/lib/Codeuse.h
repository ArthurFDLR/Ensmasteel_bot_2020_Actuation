#ifndef CODEUSE_INCLUDED
#define CODEUSE_INCLUDED

#include "Arduino.h"


class Codeuse
{
  public:
	  float dPos,pos;        //Vitesse et avance du moteur
	  void actuate(float dt);     //Actualise (transforme les ticks en vitesse puis en avance)
	  Codeuse();
	  Codeuse(bool needNormalize, uint8_t pin1,uint8_t pin2,float tickToPos);  
    void reset();
  
  private:
    bool needNormalize;
    uint8_t pin1,pin2;
	  float tickToPos;
	  int32_t ticks,oldTicks;     //Nombre de ticks compté, nombre de ticks compté au dernier appel
	  uint8_t ID;
    static uint8_t nextIdToGive;
};


#endif
