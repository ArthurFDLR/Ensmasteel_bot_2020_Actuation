#include "Codeuse.h"

int32_t ticksTab[2]={0,0};
uint8_t pins[2][2]={{0,0},{0,0}};
uint8_t Codeuse::nextIdToGive=0;

float normalizeCodeuse(float theta)
{
    float out;
    out=theta-(2*PI)*((int)(theta/(2*PI)));
    if (out>PI)
        return (out-2*PI);
    else if (out<=-PI)
        return (out+2*PI);
    return out;
}

void Codeuse::actuate(float dt)
{
  ticks= ticksTab[ID] ;
  dPos =(ticks-oldTicks)/(tickToPos*dt);
  if (needNormalize)
    pos = normalizeCodeuse(ticks/tickToPos);
  else
    pos=ticks/tickToPos;
  oldTicks= ticks;
}

void Codeuse::reset()
{
  ticksTab[ID]=0;
  pos=0;
  dPos=0;
  oldTicks=0;
  ticks=0;
}

void handler0() 
 {

  if (digitalRead(pins[0][0]) == HIGH) 
  {
    if (digitalRead(pins[0][1]) == HIGH)
      ticksTab[0] = ticksTab[0] + 1;
    else
      ticksTab[0] = ticksTab[0] - 1;
  }
  else 
  {
    if (digitalRead(pins[0][1]) == LOW)
      ticksTab[0] = ticksTab[0] + 1;
    else
      ticksTab[0] = ticksTab[0] - 1;
  }
}

void handler1() 
 {

  if (digitalRead(pins[0][0]) == HIGH) 
  {
    if (digitalRead(pins[0][1]) == HIGH)
      ticksTab[0] = ticksTab[0] - 1;
    else
      ticksTab[0] = ticksTab[0] + 1;
  }
  else 
  {
    if (digitalRead(pins[0][1]) == LOW)
      ticksTab[0] = ticksTab[0] - 1;
    else
      ticksTab[0] = ticksTab[0] + 1;
  }
}

void handler2() 
 {
  if (digitalRead(pins[1][0]) == HIGH) 
  {
    if (digitalRead(pins[1][1]) == HIGH){
      ticksTab[1] = ticksTab[1] + 1;
    }
    else
      ticksTab[1] = ticksTab[1] - 1;
  }
  else 
  {
    if (digitalRead(pins[1][1]) == LOW)
      ticksTab[1] = ticksTab[1] + 1;
    else
      ticksTab[1] = ticksTab[1] - 1;
  }
}

void handler3() 
 {
  if (digitalRead(pins[1][0]) == HIGH) 
  {
    if (digitalRead(pins[1][1]) == HIGH){
      ticksTab[1] = ticksTab[1] - 1;
      
    }
    else
      ticksTab[1] = ticksTab[1] + 1;
  }
  else 
  {
    if (digitalRead(pins[1][1]) == LOW)
      ticksTab[1] = ticksTab[1] - 1;
    else
      ticksTab[1] = ticksTab[1] + 1;
  }
}

Codeuse::Codeuse(bool needNormalize,uint8_t pin1,uint8_t pin2,float tickToPos)
{ 
  this->needNormalize=needNormalize;
  this->pin1=pin1;
  this->pin2=pin2;
//  pinMode(pin1,INPUT_PULLUP);
//  pinMode(pin2,INPUT_PULLUP);
  this->ID=nextIdToGive;
  nextIdToGive++;
  pins[ID][0]=pin1;
  pins[ID][1]=pin2;
  if (ID==0)
  {
    attachInterrupt(digitalPinToInterrupt(pin2),handler1,CHANGE);
  }
  else
  {
    attachInterrupt(digitalPinToInterrupt(pin2),handler3,CHANGE);
  }
  ticks=0;
  oldTicks=0;
  pos=0.0;
  dPos=0.0;
  this->tickToPos=tickToPos;
}
