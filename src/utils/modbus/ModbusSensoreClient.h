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

class ModbusSensorClientInterface{
public:
    ModbusSensorClientInterface(){
        //std::cout<<"Created interface" << std::endl;
    }

    virtual void ProcessSensorCallback();
    virtual void ProcessEnemySensorCallback1();
    virtual void ProcessEnemySensorCallback2();
    virtual void ProcessEnemySensorCallback3();
    virtual void ProcessEnemySensorCallback4();
    virtual void ProcessBeaconCallback();
    bool readingSensore = false ;
    bool readingBeckon = false;

    enemyDetectedStruct enemyDetected;

    int brxonValue;
    beaconDataStruct beaconData;

};

class ModbusSensorClient : public Node, public ModbusSensorClientInterface {
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
        ModbusSensorClientInterface* interface;
        beaconDataStruct beaconData;
    };

    ModbusClient* modbus;
    ModbusSensorClient();

    typedef map <idData, ModbusSensorClientInterface*> callbackRegisterMapType;
    typedef map <idData, ModbusSensorClientInterface*>::iterator it_type;
    callbackRegisterMapType callbackRegisterMap;


    mutex* m_mutex; // TODO u destruktor ubaciti hoce struha kill kill
    bool shouldStop = false;
    bool* ModbusSensoreClientNotifier;
    bool* ModbusSensorClientPanic;
    bool delayAlreadyCalculated = false;
    bool didReading = false;
    int delayTime = 20;
    int delayTime2 = 60; // 50
    int delayTime1 = 20; // 20
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
    void registerToSensoreCallback(unsigned char _slave_address, unsigned short _coil_address, bool on_bool_callback, ModbusSensorClientInterface *obj);

    short readBrxon();
    bool startBrxon();
    bool stopBrxon();

    bool startBeacon();
    bool stopBecaon();
    bool readBeacon();
    void registerToBeaconInterface(ModbusSensorClientInterface *_interface);

    //void registerToModbusRegisterCallback()
};

} // end namespace modbus

#endif // _MODBUS_SENSORE_CLIENT
