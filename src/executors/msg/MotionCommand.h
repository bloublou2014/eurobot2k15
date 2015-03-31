#ifndef _MOTIONCOMMAND_H
#define _MOTIONCOMMAND_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"

#include "drivers/motion/MotionDriver.h"

using namespace robot;
using geometry::Point2D;

namespace motion {

class MotionCommand:public Command{
public:
    static string NAME;
    enum MotionType{ MOVE_STRAIGHT,
                     ROTATE_FOR,
                     ROTATE_TO,
                     MOVE_TO_POSITION,
                     MOVE_ARC,
                     STOP,
                     SET_SPEED
                    };

    MotionCommand(MotionType _type):Command("MotionCommand", "MotionExecutor"),type(_type){}
    MotionType getMotionType() const { return type; }
private:
    MotionType type;
};

class MoveForward : public MotionCommand{
public:
    MoveForward(int _distance):MotionCommand(MOVE_STRAIGHT), distance(_distance){}

    int getDistance() const{return distance;}
private:
    int distance;
};

class RotateFor : public MotionCommand{
public:
    RotateFor(int _relativeAngle):MotionCommand(ROTATE_FOR), relativeAngle(_relativeAngle){}

    int getRelativeAngle() const{return relativeAngle;}
private:
    int relativeAngle;
};

class RotateTo : public MotionCommand{
public:
    RotateTo(int _absoluteAngle):MotionCommand(ROTATE_TO), absoluteAngle(_absoluteAngle){}

    int getAbsoluteAngle() const{return absoluteAngle;}
private:
    int absoluteAngle;
};

class MoveToPosition : public MotionCommand{
public:
    MoveToPosition(Point2D _position, MotionDriver::MovingDirection _direction= MotionDriver::MovingDirection::FORWARD):
        MotionCommand(MOVE_TO_POSITION), position(_position),direction(_direction){}

    Point2D getPosition() const {return position;}
    MotionDriver::MovingDirection getDirection() const{return direction;}
private:
    Point2D position;
    MotionDriver::MovingDirection direction;
};

class MoveArc : public MotionCommand{
public:
    MoveArc(Point2D _center, int _angle, MotionDriver::MovingDirection _direction= MotionDriver::MovingDirection::FORWARD):
        MotionCommand(MOVE_ARC),center(_center), angle(_angle), direction(_direction){}

    Point2D getCenter() const{return center;}
    int getAngle() const{return angle;}
    MotionDriver::MovingDirection getDirection() const{return direction;}
private:
    Point2D center;
    int angle;
    MotionDriver::MovingDirection direction;
};

class StopMovement : public MotionCommand{
public:
    StopMovement():MotionCommand(STOP){}
private:
};

class SetSpeedMotion :public MotionCommand{
public:
    SetSpeedMotion(int _speed):MotionCommand(SET_SPEED),speed(_speed){}
    int getSpeed(){return speed;}
private:
    int speed;
};

class MotionCommandResponse : public CommandResponse{
public:
    MotionCommandResponse(const string& to, const string& from, ResponseStatus _status=SUCCESS):CommandResponse(to,from,_status) {}
private:
};



}

#endif //_MOTIONCOMMAND_H
