#ifndef ENEMYDETECTORCOMMAND_H
#define ENEMYDETECTORCOMMAND_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"
#include "utils/helper/BeaconData.h"


//Milanovo begin
#include "messages/Command.h"
#include "messages/CommandResponse.h"
#include "utils/helper/MotionState.h"
#include "drivers/motion/MotionDriver.h"
using namespace robot;
using geometry::Point2D;
using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;
//milanovo end

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

namespace motion{
class AddStaticObject : public Command{ // done
public:
    static string NAME;
    /* Exports object */
    static void Init(Handle<Object> exports);
    /* Constructor */
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    AddStaticObject(vector<geometry::Point2D>& _edges):
        Command(NAME,"MotionExecutor"),edges(_edges){}
    AddStaticObject(const AddStaticObject& ao):Command(ao),edges(ao.edges){}
    vector<geometry::Point2D> getEdges() const {return edges;}

    Message* clone(){
        return new AddStaticObject(*this);
    }
private:
    vector<Point2D> edges;
};
}

#endif // ENEMYDETECTORCOMMAND_H
