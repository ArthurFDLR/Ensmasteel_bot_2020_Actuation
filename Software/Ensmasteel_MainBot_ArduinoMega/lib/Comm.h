#ifndef COMM_INCLUDED
#define COMM_INCLUDED
#include "Arduino.h"
#include "1_CONSTANTS.h"

class Comm
{
public:
    MessageE lastMessage=MessageE::Default;
    float posX,posY,theta;
    bool avance;
    void actuate();
    void taken();
    void send(MessageE message);
    Comm();
    AnticolE anticol;
private:


    enum StatesE{Standard,WaitingX,WaitingY,WaitingTheta,WaitingAnticol} state;
    uint32_t millisOutOfStandardState,millisLastEvitemment;
    void send(uint8_t value);
};
#endif
