#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_
#include "Arduino.h"
#include "StateName.h"
#include "Signals.h"
#include "MessageID.h"
#include "Logger.h"

class Communication;

/*
* Une transition est constitué d'un élément déclancheur
* (la reception d'un message ou la reception d'un signal (TODO ou une erreur))
* et d'un état target a rejoindre
*/
class Transition{
public:
    bool isMessage;
    union
    {
        Signal s;
        MessageID mess;
    };
    StateName target;
};

/*
* Un state est un des etats de la state machine
*/
class State
{
public:
    /*
    * Le nom du state (cf enum StateName)
    */
    StateName name;

    /*
    * Rajoute une transition sortante de cet état vers l'état cible "target" lors de la reception du signal s
    */
    void addTransitionSignal(Signal s, StateName target);

    /*
    * Rajoute une transition sortante de cet état vers l'état cible "target" lors de la reception du message mess via la communication
    */
    void addTransitionMessage(MessageID mess, StateName target);

    /*
    * Indique a la machine à état que l'on veut quitter cet état pour rejoindre l'état cible "traget"
    */
    void requestTransition(StateName target);

    /*
    * Cet méthode sera appelé lors de l'activation de ce state
    */
    virtual void onStart(){}

    /*
    * Cette méthode est appelée en boucle tant que ce state est actif
    */
    virtual void onRun(float dt){}

    /*
    * Cet méthode sera appelé lorsque l'on quitte cet état
    */
    virtual void onLeave(){}

    /*
    * Le contructeur a besoin du nom de la state et de savoir si cet état est activé dès le début (état intial)
    */
    State(StateName name, bool isStartState);

protected:
    Transition transitions[__NBSTATE__];
    uint8_t nbTransitions;
    bool started;
    bool running;
    StateName target;
    bool transitionRequested;
friend class StateMachine;
};

/*
* Une stateMachine est un ensemble d'état et de transition
*/
class StateMachine{
public:
    /*
    * Renvoie l'état désigné par le nom name
    */
    State* getStateByName(StateName name);

    /*
    * Initialise l'état "state" et l'ajoute dans le state machine.
    * /!\ doit etre appelé sur tous les états définis dans l'enum <--- TODO vérifier a postériori
    */
    void setState(State* state);

    /*
    * Actualise la machine a état (regarde si un state a reçu le bon signal ou demande a changer de state)
    */
    void update(float dt);

    /*
    * Constructeur: A besoin d'un objet pour communiquer
    */
    StateMachine(Communication* com);
    StateMachine(){};
private:
    State* states[__NBSTATE__];
    Communication* com;
};


// ================  State réutilisables  ================


class SimpleDelay : public State
{
private:
    float topStart;
    float timeToWait;
    StateName next;
public:
    SimpleDelay(StateName myName, StateName nextName,float time, bool isStartState) : State(myName,isStartState) { timeToWait=time; next = nextName;}
    void onStart() override {Logger::debugln("start sleep");topStart = millis()/1e3;}
    void onRun(float dt) override {
        if (millis()/1e3 - topStart > timeToWait)
            requestTransition(next);
    }
};


#endif // STATEMACHINE_H_