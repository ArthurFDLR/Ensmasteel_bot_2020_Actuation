#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_
#include "Arduino.h"
#include "StateName.h"
#include "Signals.h"
#include "MessageID.h"
#include "Logger.h"

class Communication;

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

class State
{
protected:
    Transition transitions[__NBSTATE__];
    uint8_t nbTransitions;
    bool started;
    bool running;
    StateName target;
    bool transitionRequested;

public:
    StateName name;
    void addTransitionSignal(Signal s, StateName target);
    void addTransitionMessage(MessageID mess, StateName target);
    void requestTransition(StateName target);
    virtual void onStart(){}
    virtual void onRun(float dt){}
    virtual void onLeave(){}
    State(StateName name, bool isStartState);

friend class StateMachine;
};

class StateMachine{
private:
    State* states[__NBSTATE__];
    Communication* com;
public:
    State* getStateByName(StateName name);
    void setState(State* state);
    void update(float dt);
    StateMachine(Communication* com);
    StateMachine(){};
};



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