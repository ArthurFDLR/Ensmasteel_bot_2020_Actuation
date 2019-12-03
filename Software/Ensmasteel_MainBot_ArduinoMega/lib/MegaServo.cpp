#include "MegaServo.h"
#include "Servo.h"
MegaServo::MegaServo(uint8_t pin,uint16_t microsRetracted,uint16_t microsHalfRetracted,uint16_t microsHalfExtended,uint16_t microsExtended)
{
  this->pin=pin;
  genuinServo.attach(pin);
  this->pos=ServoPosition::Extended;
  this->predefinedMicros[ServoPosition::Retracted]=microsRetracted;
  this->predefinedMicros[ServoPosition::HalfRetracted]=microsHalfRetracted;
  this->predefinedMicros[ServoPosition::HalfExtended]=microsHalfExtended;
  this->predefinedMicros[ServoPosition::Extended]=microsExtended;
  set(ServoPosition::Extended);
}

void MegaServo::set(ServoPosition pos)
{
  this->pos=pos;
  genuinServo.writeMicroseconds(predefinedMicros[pos]);
}

ServoPosition MegaServo::get()
{
  return pos;
}

void MegaServo::debug(int microsec)
{
  genuinServo.writeMicroseconds(microsec);
}

MegaServo::MegaServo()
{
  
}
