#ifndef PID_INCLUDED
#define PID_INCLUDED

#include "Filtre.h"

float Normalize(float x);

class PID
{
  public:
	  float I, KP, KI, KD;
	  bool needNormalizing;
	  Filtre dPosFiltered;
    void reset();

	  float compute(float dt,float aim,float pos,float dPosRaw);
	  PID(bool needNormalizing, float KP, float KI, float KD,float cutFrequency,float initValue);
	  PID();
};

#endif
