#include "Barillet.h"
#include "Arduino.h"
#include "Mega.h"
#include "Vector.h"


Barillet::Barillet(uint8_t pinContacteur,uint8_t pin1Codeuse,uint8_t pin2Codeuse,float tickToPos,uint8_t pinMoteurPwr,uint8_t pinMoteurSens,uint8_t pinMoteurBrake, Mega* ptrMega)
{
  pinMode(pinMoteurBrake,OUTPUT);
  digitalWrite(pinMoteurBrake,LOW);
  pidBarillet=PID(true,10);
  moteurBarillet=Motor(pinMoteurPwr,pinMoteurSens);
  codeuseBarillet=Codeuse(true,pin1Codeuse,pin2Codeuse,tickToPos);
  contacteurBarillet=Contacteur(pinContacteur);
  this->ptrMega=ptrMega;
  color=Color();
  accBarillet=ACCBARILLETSTD;
  endTarget=0.0;
  currentAim=0.0;
  currentDAim=0.0;
  pos=0;
  dPos=0;

  pidBarillet.setCurrentProfile(off);
}

void Barillet::actuate(float dt)
{
  if (dt>1){
    codeuseBarillet.actuate(dt);
    pos += codeuseBarillet.deltaAvance;
    return;
  }

  color.actuate();
  dPos = codeuseBarillet.v;
  pos += codeuseBarillet.deltaAvance;

  float t = millis()/1e3 - tStartMove;

  currentAim = trapez.f(t - tStartMove);
  currentDAim = trapez.df(t - tStartMove);

  
  codeuseBarillet.actuate(dt);
  moteurBarillet.order=pidBarillet.compute(currentAim,currentDAim,pos,dPos,dt)*0.85;
  moteurBarillet.actuate();
}

Barillet::Barillet(){}

void Barillet::init()
{
  float t=millis()/1000.0;
  float tIni=t;

  while (!contacteurBarillet.isPressed() and t-tIni<20)
  {
    t=millis()/1000.0;
    moteurBarillet.order=-20;   //On fait tourner le barillet doucement
    moteurBarillet.actuate();
  }

  moteurBarillet.order=0;// On arrete le barillet
  moteurBarillet.actuate();

  pos = 0;
  dPos = 0;
  pidBarillet.reset();
  pidBarillet.setCurrentProfile(standard);

  goTo(BARILLET_AngleToInit);
  
  float top = millis()/1e3;
  float lastTop = millis()/1e3;
  while (!goodenough() && t-tIni<30)
  {
    lastTop = top;
    top = millis()/1e3;
    this->actuate(top - lastTop);
  }

  pos = 0;
  dPos = 0;
  pidBarillet.reset();
}

void Barillet::goTo(float newTarget)
{
    newTarget=normalizeAngle(newTarget);
    newTarget=normalizeAngle(pos + normalizeAngle(newTarget - pos));
    endTarget = newTarget;
     if(newTarget!=endTarget)
    {
      if (normalizeAngle(newTarget-pos)>0)
        accBarillet=ACCBARILLETSTD;
      else
        accBarillet=-ACCBARILLETSTD;
      this->currentAim=pos;
      this->currentDAim=0.0;
      tStartMove=millis()/1000.0;
      trapez.set(ACCBARILLETSTD,ACCBARILLETSTD,99.0,normalizeAngle(newTarget - pos));  //<----------- Arthur check PLS (signe ?)
    }
}


void Barillet::goToDelta(float angle)
{
    goTo(normalizeAngle(pos+angle));
}

float Barillet::posCell(int cell)
{
  return cell*BARILLET_AngleToNext;
}

void Barillet::goToCell(int cell)
{
  goTo(posCell(cell));
}

bool Barillet::goodenough()
{
    return ( abs(normalizeAngle(endTarget - pos))<1.0*DEG_TO_RAD && dPos<0.5*DEG_TO_RAD && pidBarillet.isClose());
}


/*
bool Barillet::RedefinitionPosBleuium(){

  int i=1;
  bool bleutrouve=false;
  goTo(normalizeBarillet(Poscellule1+BARILLET_AngleToNext));
  while (!bleutrouve and (i<6))// on fait tourner le barillet jusqu'à trouver le palet bleu
    {
      ptrMega->actuate();

      if (goodenough()){
            moteurBarillet->order=0;   //On fait tourner le barillet doucement
            moteurBarillet->actuate();
        if (color->getPaletCouleur() ==Bleu){
            bleutrouve= true;
        }
        else
        {
        i++;
        goTo(Poscellule1 + i*BARILLET_AngleToNext);
        }
      }
    }
      if (coteviolet)
      { //si on est du coté violet on met le bleu à droite
        goTo(Poscellule1 + (i+3)*BARILLET_AngleToNext);
        while(!goodenough())
        {
          ptrMega->actuate();
        }
      }
      else
      {
        goTo(Poscellule1 + (i+4)*BARILLET_AngleToNext);
        while(!goodenough())
        {// si on est coté jaune on met le bleu à gauche
          ptrMega->actuate();
        }
    }
    pos=0;
    pidBarillet->reset();
    goTo(0.0);
  }*/
