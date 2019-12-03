#include "Pinces.h"
#include "1_CONSTANTS.h"
#include "Arduino.h"
Pinces::Pinces(bool dummy)
{
    pinceDroite=MegaServo(PINCE_DROITE_PIN,PINCE_DROITE_RETRACTED,PINCE_DROITE_HALF_RETRACTED,PINCE_DROITE_HALF_EXTENDED,PINCE_DROITE_EXTENDED);
    delay(500);
    pinceGauche=MegaServo(PINCE_GAUCHE_PIN,PINCE_GAUCHE_RETRACTED,PINCE_GAUCHE_HALF_RETRACTED,PINCE_GAUCHE_HALF_EXTENDED,PINCE_GAUCHE_EXTENDED);
    ordresDroit=ServoPosition::Extended;
    ordresGauche=ServoPosition::Extended;
    millisNextActionDroite=millis();
    millisNextActionGauche=millis();
}

Pinces::Pinces()
{
  
}

void Pinces::actuate()
{
    if (millis()>millisNextActionDroite && pinceDroite.get()!=ordresDroit)
    {
        Serial.print("pince droite ordre: ");
        Serial.println(ordresDroit);
        pinceDroite.set(ordresDroit);
    }
    if (millis()>millisNextActionGauche && pinceGauche.get()!=ordresGauche)
    {
        Serial.print("pince gauche ordre: ");
        Serial.println(ordresGauche);
        if (pinceGauche.get()==ServoPosition::Retracted && pinceDroite.get()==ServoPosition::Retracted)
        {
          Serial.println("Collision pince, abort");
          ordresGauche=pinceGauche.get();
        }
        pinceGauche.set(ordresGauche);
    }
}

void Pinces::pinceGaucheSet(ServoPosition pos, bool wait)
{
    if (wait)
        millisNextActionGauche=millis()+MILLIS_CLEAR;
    else
        millisNextActionGauche=millis()-1;

    ordresGauche=pos;
}

void Pinces::pinceDroiteSet(ServoPosition pos, bool wait)
{
    if (wait)
        millisNextActionDroite=millis()+MILLIS_CLEAR;
    else
        millisNextActionDroite=millis()-1;

    ordresDroit=pos;
}


void Pinces::bothPincesSet(ServoPosition pos)
{
    Serial.print("On me demande both : ");Serial.println(pos);
    if (pos==ServoPosition::Retracted)//On doit ranger les pinces
    {
      Serial.println("range pince");
        //TODO a modifier en fonction de la couleur
        pinceGaucheSet(ServoPosition::Retracted,false);
        pinceDroiteSet(ServoPosition::Retracted,true);
    }
    else if (pinceGauche.get()==ServoPosition::Retracted || pinceDroite.get()==ServoPosition::Retracted) //On doit sortir les pinces
    {
      Serial.println("sort pince");
        //TODO a modifier en fonction de la couleur
        pinceDroiteSet(pos,false);
        pinceGaucheSet(pos,true);
        
    }
    else
    {
      Serial.println("mouvement de pince classique");
        pinceGaucheSet(pos,false);
        pinceDroiteSet(pos,false);
    }
}
