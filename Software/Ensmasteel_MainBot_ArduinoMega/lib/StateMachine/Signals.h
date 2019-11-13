#ifndef SIGNALS_H_
#define SIGNALS_H_
#include "Arduino.h"
/*
* Les signaux sont des messages internes
*/
enum Signal
{
    _noSignal_,
    TouchedS,
    OkS,
    TiretteS
};

class SignalBox;

/*
* Classe statique de gestion des signaux
* Les signaux sont des messages échangés en interne (au sein du programme)
*/
class SignalManager
{
public:
    /*
    * emet le signal s
    * N'importe quel state peut alors le voir et réagir
    */
    static void emit(Signal s);

    /*
    * Regarde le plus vieux signal (les trop vieux (cf maxAge) n'y sont plus)
    */
    static Signal peekOldestSignal();

    /*
    * Retire le plus vieux signal
    */
    static void popOldest();

    /*
    * Nombre de signaux en attente
    */
    static uint8_t inWaiting();

    /*
    * Intialisation du SignalManager
    */
    static void setup();

    /*
    * Parcours les sigaux en attente.
    * Les signaux on un age 0 à leur arrivée
    * A chaque appel de cette fonction, les signaux gagne 1 d'age.
    * Si le signal le plus vieux a un age > à maxAge alors il est supprimé
    */
    static void clean(int maxAge);

private:
    static SignalBox* signalBox;
};

#endif // !SIGNALS_H_
