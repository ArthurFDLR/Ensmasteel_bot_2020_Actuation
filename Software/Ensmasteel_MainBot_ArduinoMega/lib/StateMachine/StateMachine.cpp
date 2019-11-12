#include "StateMachine.h"
#include "Logger.h"
#include "Communication.h"


void State::addTransitionSignal(Signal s, StateName target) {
    if (nbTransitions==__NBSTATE__){
            Logger::infoln("ERROR : too much transition");
    }
    else
    {
        transitions[nbTransitions].isMessage = false;
        transitions[nbTransitions].s = s;
        transitions[nbTransitions].target = target;
        nbTransitions++;
    }
}

void State::addTransitionMessage(MessageID mess, StateName target) {
    if (nbTransitions==__NBSTATE__){
            Logger::infoln("ERROR : too much transition");
    }
    else
    {
        transitions[nbTransitions].isMessage = true;
        transitions[nbTransitions].mess = mess;
        transitions[nbTransitions].target = target;
        nbTransitions++;
    }
}

State::State(StateName name, bool isStartState) {
    nbTransitions = 0;
    this->running = isStartState;
    this->name = name;
    this->started = false;
    this->transitionRequested = false;
}

void State::requestTransition(StateName target)
{
    transitionRequested = true;
    this->target = target;
}

StateMachine::StateMachine(Communication* com)
{
    this->com = com;
}

State* StateMachine::getStateByName(StateName name) {
    return states[(int)name];
}

void StateMachine::setState(State* state) {
    states[int(state->name)] = state;
}

void StateMachine::update(float dt) {
    Signal s = Signal::_noSignal_;
    if (SignalManager::inWaiting()>0)
        s = SignalManager::peekOldestSignal();

    for (int i=0;i<__NBSTATE__;i++) //Pour chaque etat
    {
        if (states[i]->running) //Si il est actif
        {
            if (!states[i]->started) //Si il n'a pas encore été intialisé
            {
                states[i]->onStart(); //On fait l'action de départ
                states[i]->started=true;
            }

            states[i]->onRun(dt); //On fait les actions classiques

            for (int t = 0; t<states[i]->nbTransitions;t++) //Pour chaque transition
            {
                if ( (com->inWaitingRx()>0 && states[i]->transitions[t].isMessage && states[i]->transitions[t].mess == extractID(com->peekOldestMessage())) //Si on a le bon message
                ||   (!states[i]->transitions[t].isMessage && states[i]->transitions[t].s == s)) // Si on a le bon signal
                {
                    getStateByName(states[i]->transitions[t].target)->running = true; //On active la target
                    states[i]->onLeave();
                    states[i]->started = false;
                    states[i]->transitionRequested = false;
                    states[i]->running = false; //On endort cete action
                }
            }

            if (states[i]->transitionRequested)
            {
                getStateByName(states[i]->target)->running = true; //On active la target
                states[i]->onLeave();
                states[i]->started = false;
                states[i]->transitionRequested = false;
                states[i]->running = false; //On endort cete action
            }
            
        }
    }
}
