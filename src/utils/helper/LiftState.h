#ifndef _LIFTSTATE_H
#define _LIFTSTATE_H

namespace executor {

struct LiftState{
    LiftState(){}
    LiftState(int _quantity, bool _aveable): Quantity(_quantity), Aveable(_aveable){}

    int Quantity;
    bool Aveable;
};
}


#endif // _LIFTSTATE_H
