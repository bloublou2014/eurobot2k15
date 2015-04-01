#ifndef _MOTIONSTATE_H
#define _MOTIONSTATE_H

#include "utils/geometry/Point2D.h"
#include "drivers/motion/MotionDriver.h"

using namespace geometry;

namespace motion{

struct MotionState{
    MotionState(){}
    MotionState(char _speed, geometry::Point2D _position, int _orientation, int _direction):
    Speed(_speed),Position(_position),Orientation(_orientation),Direction(_direction){}

    char Speed;
    geometry::Point2D Position;
    MotionDriver::State State;
    int Orientation;
    int Direction;

    bool operator==(const MotionState &m) const;
    bool operator!=(const MotionState &m) const;
};

}

#endif //_MOTIONSTATE_H
