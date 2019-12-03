#include "PID.h"
#define RECONVERGENCE 0.05
//SI le robot est eloigné de plus de RECONVERGENCE metres, le PID angulaire s'occupe a 100% de rejoindre le ghost, pas de mimer le theta


void PID::reset()
{
    this->iTerm = 0;
}

void PID::setCurrentProfile(MoveProfileName name){
    this->currentProfile = MoveProfiles::get(name,!modulo360);
}

MoveProfile* PID::getCurrentProfile(){
    return currentProfile;
}

float PID::compute(float xTarget, float dxTarget, float x, float dx, float dt)
{
    float error;
    float dError;

    if (modulo360)
        error = normalizeAngle(xTarget - x);
    else
        error = xTarget - x;

    dxF.in(dx, dt);
    dError = dxTarget - dxF.out();

    iTerm += error * dt;

    close = (abs(error) <= currentProfile->epsilon) && (abs(dError) <=currentProfile->dEpsilon);


    float out = constrain(
        currentProfile->KP * error + currentProfile->KI * iTerm + currentProfile->KD * (currentProfile->KA*dxTarget - dxF.out()), -1.0, 1.0);

    return out;
}

PID::PID(bool modulo360, float frequency)
{
    this->currentProfile = 0;
    this->dxF = Filtre(0, frequency);
    this->close = true;
    this->modulo360 = modulo360;
}

PID::PID() {}