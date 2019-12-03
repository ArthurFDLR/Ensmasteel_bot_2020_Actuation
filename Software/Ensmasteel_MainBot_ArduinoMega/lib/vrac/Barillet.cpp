#include "Barillet.h"
#include "Arduino.h"
#include "Mega.h"
float normalizeBarillet(float theta)
{
    float out;
    out=theta-(2*PI)*((int)(theta/(2*PI)));
    if (out>PI)
        return (out-2*PI);
    else if (out<=-PI)
        return (out+2*PI);
    return out;
}


Barillet::Barillet(uint8_t pinContacteur,uint8_t pin1Codeuse,uint8_t pin2Codeuse,float tickToPos,uint8_t pinMoteurPwr,uint8_t pinMoteurSens,uint8_t pinMoteurBrake,bool coteviolet,Mega* ptrMega)
{
  pidBarillet=new PID(true,10);
  moteurBarillet=new Motor(pinMoteurPwr,pinMoteurSens,pinMoteurBrake);
  codeuseBarillet=new Codeuse(true,pin1Codeuse,pin2Codeuse,tickToPos);
  contacteurBarillet=new Contacteur(pinContacteur);
  this->coteviolet=coteviolet;
  this->ptrMega=ptrMega;
  color=new Color(true);
  accBarillet=ACCBARILLETSTD;
  aim=0.0;
  dAim=0.0;
  pos=0;
  dPos=0;
}

void Barillet::actuate(float dt)
{
  if (dt>1){
    dt=0.01;
  }

  color->actuate();
  dPos = codeuseBarillet->v;
  pos += codeuseBarillet->deltaAvance;

  if (millis()/1000.0<tInversion)
  {
    dAim+=accBarillet*dt;
    aim=normalizeBarillet(aim+dAim*dt);
  }
  else if(millis()/1000.0<tFinGoto)
  {
      dAim-=accBarillet*dt;
      aim=normalizeBarillet(aim+dAim*dt);
  }
  else
  {
      dAim=0.0;
      aim=target;
  }
  codeuseBarillet->actuate(dt);
  moteurBarillet->order=pidBarillet->compute(aim,dAim,pos,dPos,dt)*0.85;
  moteurBarillet->actuate();

}

Barillet::Barillet()
{

}

bool Barillet::init()
{
  float t=millis()/1000.0;
  float tIni=t;

  while (!contacteurBarillet->isPressed() and t-tIni<20)
  {
    t=millis()/1000.0;
    moteurBarillet->order=-20;   //On fait tourner le barillet doucement
    moteurBarillet->actuate();
  }

  moteurBarillet->order=0;// On arrete le barillet
  moteurBarillet->actuate();
  pos = 0;
  dPos = 0;
  pidBarillet->reset();
  goTo(0.0);


  // definition des positions du barillet par rapport à l'origine
  Poscellule1 = 0;
  Poscellule2 = normalizeBarillet(BARILLET_AngleToNext);
  Poscellule3 = normalizeBarillet(2*BARILLET_AngleToNext);
  Poscellule4 = normalizeBarillet(3*BARILLET_AngleToNext);
  Poscellule5 = normalizeBarillet(4*BARILLET_AngleToNext);
  Poscellule6 = normalizeBarillet(5*BARILLET_AngleToNext);

  if (t-tIni>=20)return false;
  else
  {
    goTo(BARILLET_AngleToInit);
    return true;
  }
}

//void Barillet::addPalet(int cellId, PaletE palet)
//{
//	cellule[cellId]->add(palet);
//}
//
void Barillet::goTo(float angle)
{
    angle=normalizeBarillet(angle);
    angle=normalizeBarillet(pos + normalizeBarillet(angle - pos));
     if(angle!=target)
    {
      if (normalizeBarillet(angle-pos)>0)
        accBarillet=ACCBARILLETSTD;
      else
        accBarillet=-ACCBARILLETSTD;
      this->aim=pos;
      this->dAim=0.0;
      tStartGoto=millis()/1000.0;
      tFinGoto=tStartGoto + sqrt(abs(4.0*normalizeBarillet(angle-pos))/ACCBARILLETSTD);
      tInversion=(tFinGoto+tStartGoto)/2.0;
      target=angle;
    }
}

void Barillet::goToDelta(float angle)
{
      angle=normalizeBarillet(normalizeBarillet(angle)+normalizeBarillet(target));
    if (angle!=target)
    {
      if (normalizeBarillet(angle-pos)>0)
        accBarillet=ACCBARILLETSTD;
      else
        accBarillet=-ACCBARILLETSTD;
      this->aim=pos;
      this->dAim=0.0;
      tStartGoto=millis()/1000.0;
      tFinGoto=tStartGoto + sqrt(abs(4.0*(normalizeBarillet(angle-pos))/ACCBARILLETSTD));
      tInversion=(tFinGoto+tStartGoto)/2.0;
      target=angle;
    }
}



bool Barillet::goodenough()
{
    return (abs(normalizeBarillet(target - pos))<0.02 && dPos<0.02);
}



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
  }
