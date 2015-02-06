#ifndef _SERVOCOMMAND_H
#define _SERVOCOMMAND_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"

#include "drivers/actuators/Servos.h"


using namespace robot;


namespace servo{

class ServoCommand:public Command{
public:
    static string NAME;
    enum ServoType{
        ROTATE_FOR,
        ROTATE_TO_POSITION,
        SET_SPEED};

    ServoCommand(ServoType _type):Command("ServoCommand", "ServoExecutor"), type(_type){}
    ServoType getServoType() const {return type;}

private:
    ServoType type;
};

class RotateFor: public ServoCommand{
public:
    RotateFor(short _steps):ServoCommand(ROTATE_FOR), steps(_steps){}

    short getSteps() const {return steps; }

 private:
    short steps;
};

class RotateToPosition: public ServoCommand{
public:
    RotateToPosition(short _position):ServoCommand(ROTATE_TO_POSITION), position(_position){}

    short getPosition()const {return position;}
private:
    short position;

};

class SetSpeed: public ServoCommand{
public:
    SetSpeed(short _speed):ServoCommand(SET_SPEED), speed(_speed){}

    short getSpeed() const {return speed;}

private:
    short speed;
};


}

#endif
