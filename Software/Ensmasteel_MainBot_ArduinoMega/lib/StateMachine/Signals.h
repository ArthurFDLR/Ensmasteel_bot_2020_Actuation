#ifndef SIGNALS_H_
#define SIGNALS_H_
#include "Arduino.h"
/*
* Les signaux sont des messages sans 
*/
enum Signal
{
    _noSignal_,
    TouchedS,
    OkS,
    TiretteS
};

class SignalBox;

class SignalManager
{
public:
    static void emit(Signal s);
    static Signal peekOldestSignal();
    static void popOldest();
    static uint8_t inWaiting(); //Renvoie le nombre de message en attente dans la boite de reception
    static void setup();
    static void clean(int maxAge);

private:
    static SignalBox* signalBox;
};

#endif // !SIGNALS_H_
