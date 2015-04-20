#ifndef _MODBUS_SENSORE_CLIENT
#define _MODBUS_SENSORE_CLIENT

#include "ModbusClient.h"
#include <boost/thread/thread.hpp>
#include "core/Node.h"
#include <map>
#include <queue>
#include <iterator>
#include "utils/helper/BeaconData.h"


using boost::mutex;
using namespace robot;
namespace modbus {
/*
class ModbusSensorClientInterface{

public:

    ModbusSensorClientInterface(){
        std::cout<<"Created interface" << std::endl;
    }

    virtual void ProcessLiftLeftSensoreCallback();
    virtual void ProcessLiftRightSensoreCallback();
    virtual void ProcessSensorCallback();
    virtual void ProcessEnemySensorCallback1();
    virtual void ProcessEnemySensorCallback2();
    virtual void ProcessEnemySensorCallback3();
    virtual void ProcessEnemySensorCallback4();
    virtual void ProcessBeaconCallback();
    bool readingSensore = false ;
    bool readingBeckon = false;

    enemyDetectedStruct enemyDetected;

    //int brxonValue;
    beaconDataStruct beaconData;

};
*/

class ModbusSensorClientInterface2{
public:

    ModbusSensorClientInterface2(){
        std::cout << " CREATE INTERFACE" << std::endl;
    }
    virtual void ProcessLiftLeftSensoreCallback(){}
    virtual void ProcessLiftRightSensoreCallback(){}
    virtual void ProcessSensorCallback(){}
    virtual void ProcessEnemySensorCallback1(){}
    virtual void ProcessEnemySensorCallback2(){}
    virtual void ProcessEnemySensorCallback3(){}
    virtual void ProcessEnemySensorCallback4(){}

    virtual void ProcessNotEnemySensorCallback1(){}
    virtual void ProcessNotEnemySensorCallback2(){}
    virtual void ProcessNotEnemySensorCallback3(){}
    virtual void ProcessNotEnemySensorCallback4(){}

    virtual void ProcessBeaconCallback(){}
    bool readingSensore = false ;
    bool readingBeckon = false;

    enemyDetectedStruct enemyDetected;

    //int brxonValue;
    beaconDataStruct beaconData;
};

class ModbusSensorClient : public Node,/* public ModbusSensorClientInterface,*/ public ModbusSensorClientInterface2 {
private:

    struct setSingleRegisterData{
        idData ID;
        short data;
    };

    struct becaonStruct{
        unsigned char slaveAddress;
        short functionAddress_Start;
        short functionAddress_NumberOfBeacons;
        short functionAddress_XdataBecaon1;
        short functionAddress_YdataBeacon1;
        short functionAddress_XdataBecaon2;
        short functionAddress_YdataBeacon2;
        bool beaconON;
        int beaconDataCounter;
        bool BeaconDoneReading;
        ModbusSensorClientInterface2* interface;
        beaconDataStruct beaconData;
    };

    ModbusClient* modbus;
    ModbusSensorClient();

    typedef map <idData, ModbusSensorClientInterface2*> callbackRegisterMapType;
    typedef map <idData, ModbusSensorClientInterface2*>::iterator it_type;
    callbackRegisterMapType callbackRegisterMap;


    mutex* m_mutex; // TODO u destruktor ubaciti hoce struha kill kill
    bool shouldStop = false;
    bool* ModbusSensoreClientNotifier;
    bool* ModbusSensorClientPanic;
    bool delayAlreadyCalculated = false;
    bool didReading = false;
    int delayTime = 10;
    int delayTime2 = 20; // 50
    int delayTime1 = 12; // 20
    int timerForDelayTime = 0;
    int respinNumber; // preracuna se kod inicijalicacije

    idData brxonID;
    idData brxonStartID;

    becaonStruct beaconID;
    int beaconReading = 0;

    void main();
    void calculateDelayTime();


    friend bool operator< (const idData &id1, const idData &id2);

public:
    static ModbusSensorClient* instance;
    static ModbusSensorClient* getModbusSensorInstance();
    void stopModbusSensorClient();
    void registerToSensoreCallback(unsigned char _slave_address, unsigned short _coil_address, bool on_bool_callback,
                                   ModbusSensorClientInterface2 *obj);

    short readBrxon();
    bool startBrxon();
    bool stopBrxon();

    bool startBeacon();
    bool stopBecaon();
    bool readBeacon();
    void registerToBeaconInterface(ModbusSensorClientInterface2 *_interface);

    //void registerToModbusRegisterCallback()
};

} // end namespace modbus

#endif // _MODBUS_SENSORE_CLIENT
