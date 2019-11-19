#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_
#include "Arduino.h"
#include "MessageID.h"
#include "Logger.h"
#include "Communication.h"

#define NBMAXSTATES 100

class StateMachine;

/*
* Un state est un des etats de la state machine
* Chaque state ne connait que 3 autres state. 
* Le state suivant standard
* Le state suivant en cas d'erreur
* Et le state suivant auxiliaire.
* (Ce ne sont que des appellations)
*/
class State
{
public:
    State(float timeout);

    void setStateStd(State* state);
    void setStateErr(State* state);
    void setStateAux(State* state);
    State* getStateStd();
    State* getStateErr();
    State* getStateAux();

    //Appelé a l'activation de l'état
    virtual void onStart(){}

    //Appelé tant que l'état est actif
    virtual void onRun(float dt){}

    //Appelé quand l'état s'arrete
    virtual void onLeave(){}

    //Appelé sur demande
    virtual void onGripConfirmed(){}
    virtual void onMessageReceived(Message mess){}
    virtual void onTirettePulled(){}
    virtual void onObstacleFound(){}
    virtual void onTimeout(){}
    
    uint8_t id;

protected:
    void start();
    void stop();
    bool started;
    float topStart;
    float timeout;
    
    static uint8_t numInstance;
    StateMachine* stateMachine;
private:
    State *stateStd,*stateErr,*stateAux;
friend class StateMachine;
};

/*
* Une stateMachine est un ensemble d'états
*/
class StateMachine{
public:
    void addState(State* state);

    void update(float dt);
    void changeCurrentState(State* newState);

    void gripConfirmed(){currentState->onGripConfirmed();}
    void messageReceived(Message mess){currentState->onMessageReceived(mess);}
    void tirettePulled(){currentState->onTirettePulled();}
    void obstacleFound(){currentState->onObstacleFound();}

    StateMachine(Communication* com);
    void setStartState(State* state);
    StateMachine(){};
    Communication* com;
private:
    State* currentState;
    State* states[NBMAXSTATES];
    uint8_t nbStates;
};


//STATE GENERIQUES

class SimpleDelay : public State
{
public:
    SimpleDelay(float time) : State(time){}

    void onTimeout() override{
        stateMachine->changeCurrentState(getStateStd());
    }
};


#endif // STATEMACHINE_H_