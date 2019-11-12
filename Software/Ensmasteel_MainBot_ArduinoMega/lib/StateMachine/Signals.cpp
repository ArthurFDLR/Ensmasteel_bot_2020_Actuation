#include "Signals.h"
#include "Logger.h"

#define SIGNAL_BOX_SIZE 20

struct AgedSignal
{
    Signal s;
    int age=0;
};

class SignalBox
{
public:
    Signal pull();
    Signal peek();
    void push(Signal s);
    int size();
    bool empty = true;
    /*
    * Retire tous les elements qui sont strictement  plus vieux que maxAge
    */
    void clean(int maxAge);

private:
    AgedSignal box[SIGNAL_BOX_SIZE];
    uint8_t iFirstEntry = 0;
    uint8_t iNextEntry = 0;
};

SignalBox* SignalManager::signalBox;

Signal SignalBox::pull()
{
    if (empty)
    {
        Logger::infoln("The signalbox is empty");
        //Dans ce cas on renvoie NO_ERROR
        return Signal::_noSignal_;
    }
    else
    {
        //On recupere l'erreur et on fait avancer le buffer
        AgedSignal out = box[iFirstEntry];
        iFirstEntry = (iFirstEntry + 1) % SIGNAL_BOX_SIZE;
        empty = (iFirstEntry == iNextEntry);
        return out.s;
    }
}

Signal SignalBox::peek()
{
    if (empty)
    {
        Logger::infoln("The signalbox is empty");
        //Dans ce cas on renvoie NO_ERROR
        return Signal::_noSignal_;
    }
    else
        return box[iFirstEntry].s;
}

void SignalBox::push(Signal s)
{
    if ((iFirstEntry == iNextEntry) && !empty)
        Logger::infoln("The signalbox is full");
    //Dans ce cas on n'empile pas
    else
    {
        //Sinon on empile et on fait avancer l'indice de la prochaine entrée
        AgedSignal as; as.s=s; as.age=0;
        box[iNextEntry] = as;
        iNextEntry = (iNextEntry + 1) % SIGNAL_BOX_SIZE;
        empty = false;
    }
}

int SignalBox::size()
{
    if (empty)
        return 0;
    else if (iNextEntry == iFirstEntry)
        return SIGNAL_BOX_SIZE;
    else
        return (iNextEntry - iFirstEntry + SIGNAL_BOX_SIZE) % SIGNAL_BOX_SIZE; //Marche dans tous les cas
}

void SignalBox::clean(int maxAge)
{
    for (int i=0;i<size();i++)
        box[(iFirstEntry + i)% SIGNAL_BOX_SIZE].age++;

    if (size()>0)
    {
        if (box[iFirstEntry].age>maxAge)
            pull();
    }

}


void SignalManager::emit(Signal s) {
    signalBox->push(s);
}

Signal SignalManager::peekOldestSignal() {
    return signalBox->peek();
}

void SignalManager::popOldest() {
    signalBox->pull();
}

uint8_t SignalManager::inWaiting(){
    return signalBox->size();
}

void SignalManager::setup(){
    signalBox = new SignalBox();
}

void SignalManager::clean(int ageMax){
    signalBox->clean(ageMax);
}
