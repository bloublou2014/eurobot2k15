#ifndef _MOTIONCOMMAND_H
#define _MOTIONCOMMAND_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"

#include "drivers/motion/MotionDriver.h"
using namespace robot;
using geometry::Point2D;
using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;

namespace motion {

class MotionCommand:public Command{
public:
    static string NAME;
    enum MotionType{ MOVE_STRAIGHT,
                     ROTATE_FOR,
                     ROTATE_TO,
                     MOVE_TO_POSITION,
                     MOVE_ARC,
                     SET_SPEED,
                     SET_POSITION,
                     STOP};

    MotionCommand(MotionType _type):Command("MotionCommand", "MotionExecutor"),type(_type){}
    MotionCommand(const MotionCommand& mc):Command(mc),type(mc.type){}
    MotionType getMotionType() const { return type; }

    Message* clone(){
        return new MotionCommand(*this);
    }
private:
    MotionType type;
};

class MoveForward : public MotionCommand{ // done
public:
    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    MoveForward(int _distance):MotionCommand(MOVE_STRAIGHT), distance(_distance){}
    MoveForward(const MoveForward& mf):MotionCommand(mf),distance(mf.distance){}
    int getDistance() const{return distance;}

    Message* clone(){
        return new MoveForward(*this);
    }
private:
    int distance;
};

class RotateFor : public MotionCommand{ // done
public:
    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    RotateFor(int _relativeAngle):MotionCommand(ROTATE_FOR), relativeAngle(_relativeAngle){}
    RotateFor(const RotateFor& rf):MotionCommand(rf),relativeAngle(rf.relativeAngle){}
    int getRelativeAngle() const{return relativeAngle;}

    Message* clone(){
        return new RotateFor(*this);
    }
private:
    int relativeAngle;
};

class RotateTo : public MotionCommand{ // done
public:
    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);


    RotateTo(int _absoluteAngle):MotionCommand(ROTATE_TO), absoluteAngle(_absoluteAngle){}
    RotateTo(const RotateTo& rt):MotionCommand(rt),absoluteAngle(rt.absoluteAngle){}
    int getAbsoluteAngle() const{return absoluteAngle;}

    Message* clone(){
        return new RotateTo(*this);
    }
private:
    int absoluteAngle;
};

class MoveToPosition : public MotionCommand{ // done
public:
    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);


    MoveToPosition(Point2D _position, MotionDriver::MovingDirection _direction= MotionDriver::MovingDirection::FORWARD):
        MotionCommand(MOVE_TO_POSITION), position(_position),direction(_direction){}
    MoveToPosition(const MoveToPosition& mtp):MotionCommand(mtp),position(mtp.position),direction(mtp.direction){}
    Point2D getPosition() const {return position;}
    MotionDriver::MovingDirection getDirection() const{return direction;}

    Message* clone(){
        return new MoveToPosition(*this);
    }
private:
    Point2D position;
    MotionDriver::MovingDirection direction;
};

class MoveArc : public MotionCommand{ // done
public:
    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);


    MoveArc(Point2D _center, int _angle, MotionDriver::MovingDirection _direction= MotionDriver::MovingDirection::FORWARD):
        MotionCommand(MOVE_ARC),center(_center), angle(_angle), direction(_direction){}
    MoveArc(const MoveArc& ma):MotionCommand(ma),center(ma.center),angle(ma.angle),direction(ma.direction){}
    Point2D getCenter() const{return center;}
    int getAngle() const{return angle;}
    MotionDriver::MovingDirection getDirection() const{return direction;}

    Message* clone(){
        return new MoveArc(*this);
    }
private:
    Point2D center;
    int angle;
    MotionDriver::MovingDirection direction;
};

class StopMovement : public MotionCommand{ //done
public:
    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    StopMovement():MotionCommand(STOP){}
    StopMovement(const StopMovement& sm):MotionCommand(sm){}

    Message* clone(){
        return new StopMovement(*this);
    }
private:
};

class SetSpeedMotion : public MotionCommand{ // done
public:
    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    SetSpeedMotion(unsigned char _speed):MotionCommand(SET_SPEED), speed(_speed){}
    SetSpeedMotion(const SetSpeedMotion& rt):MotionCommand(rt),speed(rt.speed){}
    unsigned char getSpeed() const{return speed;}

    Message* clone(){
        return new SetSpeedMotion(*this);
    }
private:
    unsigned char speed;
};

class SetPosition : public MotionCommand{ // done
public:
    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    SetPosition(int x, int y, int _orientation=0):
        MotionCommand(SET_POSITION),point(x,y),orientation(_orientation){}
    SetPosition(const SetPosition& ss):MotionCommand(ss),point(ss.point),orientation(ss.orientation){}
    Point2D getPoint() const{return point;}
    int getOrientation() const {return orientation;}

    Message* clone(){
        return new SetPosition(*this);
    }
private:
    Point2D point;
    int orientation;
};

//class MotionCommandError : public CommandResponse{
//public:
//    enum ErrorType{
//        ENEMY=1,
//        STUCK=2,
//        UART=3
//    };

//    static string NAME;
//    /* Exports object */
//    static void Init(Handle<Object> exports);
//    /* Constructor */
//    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

//    MotionCommandError(ErrorType _type, const string& to, ResponseStatus _status=ERROR):
//        CommandResponse(NAME, to, "MotionExecutor", _status),errorType(_type) {}
//    MotionCommandError(const MotionCommandError& mcr):CommandResponse(mcr){}

//    Message* clone(){
//        return new MotionCommandError(*this);
//    }
//private:
//    ErrorType errorType;
//};

}

#endif //_MOTIONCOMMAND_H
