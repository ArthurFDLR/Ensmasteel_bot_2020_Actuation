#ifndef MEGASERVO_INCLUDED
#define MEGASERVO_INCLUDED
#include "Servo.h"

enum ServoPosition {Retracted,HalfRetracted,HalfExtended,Extended};
class MegaServo
{
  public:
  MegaServo(uint8_t pin,uint16_t microsRetracted,uint16_t microsHalfRetracted,uint16_t microsHalfExtended,uint16_t microsExtended);
  MegaServo();
  void set(ServoPosition pos);
  void debug(int microsec);
  ServoPosition get();
  
  private:
  Servo genuinServo;
  uint8_t pin;
  ServoPosition pos;
  uint16_t predefinedMicros[4];
};


#endif
