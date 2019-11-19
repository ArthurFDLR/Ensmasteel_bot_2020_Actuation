#include "StateMachine.h"
#include "Logger.h"
#include "Communication.h"
#include <Arduino.h>

uint8_t State::numInstance = 0;

State::State(float timeout)
{
    this->id = numInstance;
    this->started = false;
    this->timeout = timeout;   
    this->stateMachine = nullptr;
    this->stateStd = nullptr;
    this->stateErr = nullptr;
    this->stateAux = nullptr;
    numInstance++;
}

void State::setStateStd(State* state){this->stateStd = state;}
void State::setStateErr(State* state){this->stateErr = state;}
void State::setStateAux(State* state){this->stateAux = state;}

State* State::getStateStd(){
    if (stateStd == nullptr) 
        Logger::infoln("Try to call a null state") ;
    else
        Logger::debugln("Calling standard follower");
    return stateStd;
}

State* State::getStateErr(){
    if (stateErr == nullptr) 
        Logger::infoln("Try to call a null state") ;
    else
        Logger::debugln("Calling err follower");
    return stateErr;
}

State* State::getStateAux(){
    if (stateAux == nullptr) 
        Logger::infoln("Try to call a null state") ;
    else
        Logger::debugln("Calling aux follower");
    return stateAux;
}

void State::start(){
    Logger::debugln("State " + String(id) + " started");
    started = true;
    topStart = millis()/1e3;
    onStart();
}

void State::stop(){
    Logger::debugln("State " + String(id) + " stopped");
    started = false;
    onLeave();
}

void StateMachine::changeCurrentState(State* newState)
{
    currentState->stop();
    currentState = newState;
    currentState->start();
}

void StateMachine::update(float dt)
{
    if (currentState->timeout>0 && (millis()/1e3 - currentState->topStart) > currentState->timeout)
        currentState->onTimeout();

    currentState->onRun(dt);
}

StateMachine::StateMachine(Communication* com)
{
    this->com = com;
    this->nbStates = 0;
}

void StateMachine::addState(State* state)
{
    state->stateMachine = this;
    this->states[nbStates] = state;
    nbStates++;
}

void StateMachine::setStartState(State* state)
{
    currentState = state;
    currentState->start();
}