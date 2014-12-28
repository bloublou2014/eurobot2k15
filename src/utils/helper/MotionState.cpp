#include "MotionState.h"

namespace motion{

bool MotionState::operator==(const MotionState &m) const{
    if (Speed!=m.Speed) return false;
    if (!(Position==m.Position)) return false;
    if (State!=m.State) return false;
    if (Orientation!=m.Orientation) return false;
    if (Direction!=m.Direction) return false;

    return true;
}

bool MotionState::operator!=(const MotionState &m) const{
    return !(m==*this);
}

}
