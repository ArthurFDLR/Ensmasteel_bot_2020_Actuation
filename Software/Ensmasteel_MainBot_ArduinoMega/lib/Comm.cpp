#include "Comm.h"
#include "Arduino.h"

void Comm::actuate()
{
    if (state!=StatesE::Standard and (millis()-millisOutOfStandardState)>1000)
        state=StatesE::Standard;
    if (Serial2.available()>=1)
    {
        switch(state)
        {
        case StatesE::Standard:
            lastMessage=(MessageE)Serial2.read();
            if((uint8_t)lastMessage>=NB_MESSAGES)
            {
                #ifdef STATE
                Serial.print("On a recu de le merde, c'est a dire :");
                Serial.println(lastMessage);
                #endif
                lastMessage=MessageE::Default;    //On a recu de la merde
            }
            if (lastMessage==MessageE::Sync)
            {
                lastMessage=MessageE::Default;  //Les informations ne sont pas encore prete a etre lues
                state=StatesE::WaitingX;
                millisOutOfStandardState=millis();
            }
            break;

        case StatesE::WaitingX:
            posX=((float)Serial2.read())*3.0/255.0;  //Produit en croix
            state=StatesE::WaitingY;
            break;
        case StatesE::WaitingY:
            posY=((float)Serial2.read())*2.0/255.0;  //Produit en croix
            state=StatesE::WaitingTheta;
            break;
        case StatesE::WaitingTheta:
            theta=((float)Serial2.read())*2*PI/255.0-PI;  //Produit en croix
            state=StatesE::WaitingAnticol;
            break;
        case StatesE::WaitingAnticol:
            anticol=((AnticolE)Serial2.read());
            state=StatesE::Standard;
#ifdef STATE
            Serial.println("message sync recu");
            Serial.print("X : ");
            Serial.print(posX);
            Serial.print("\tY : ");
            Serial.print(posY);
            Serial.print("\tTheta : ");
            Serial.print(theta);
            Serial.print("\tanticol : ");
            Serial.println(anticol);
#endif
            break;
        }

#ifdef STATE
        //Serial.print("lastMessage ");
        //Serial.println((uint8_t)lastMessage);
#endif
    }
}

void Comm::taken()
{
    lastMessage=MessageE::Default;
}

void Comm::send(MessageE message)
{
    Serial2.write((uint8_t)message);
}

void Comm::send(uint8_t value)
{
    Serial2.write(value);
}

Comm::Comm()
{
    while(Serial2.available()>0)
    {
        Serial2.read();
    }
    state=StatesE::Standard;
    anticol=AnticolE::No;
}
