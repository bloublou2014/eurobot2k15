#ifndef ENEMYDETECTORCOMMAND_H
#define ENEMYDETECTORCOMMAND_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"
#include "utils/helper/BeaconData.h"
using namespace modbus;

namespace robot{

class SensorCommand: public Command {
public:
    static string NAME;

    SensorCommand(int _id):Command(SensorCommand::NAME,"EnemyDetectorExecutor"), sensorID(_id){}
    ~SensorCommand(){}

    int getSensorID(){return sensorID;}
private:
    int sensorID;
};

class BrkonCommand: public Command{
public:
    static string NAME;

    BrkonCommand(int _angle):Command(SensorCommand::NAME,"EnemyDetectorExecutor"), angle(_angle){}
private:
    int angle;

};

class BeaconCommand: public Command{
public:

    static string NAME;
    BeaconCommand(int _velikiCordX, int _velikiCordY, int _maliCordX, int _maliCordY):
        Command(BeaconCommand::NAME, "EnemyDetectorExecutor"),
        velikiCordX(_velikiCordX), velikiCordY(_velikiCordY), maliCordX(_maliCordX), maliCordY(_maliCordY){}

    beaconDataStruct getBeaconData(){
        beaconDataStruct data;
        data.X_beacon1 = velikiCordX;
        data.Y_beacon1 = velikiCordY;
        data.X_beacon2 = maliCordX;
        data.Y_beacon2 = maliCordY;
        return data;
    }

private:
    int velikiCordX;
    int velikiCordY;
    int maliCordX;
    int maliCordY;
};


} // end robot namespace
#endif // ENEMYDETECTORCOMMAND_H
