#ifndef _MODBUS_SENSORE_CLIENT
#define _MODBUS_SENSORE_CLIENT

#include "ModbusClient.h"
#include <boost/thread/thread.hpp>
#include "core/Node.h"
#include <map>
#include <queue>
#include <iterator>


using boost::mutex;
using namespace robot;
namespace modbus {

class ModbusSensorClientInterface{
public:
    ModbusSensorClientInterface(){
        std::cout<<"Created interface" << std::endl;
    }

    virtual void ProcessSensorCallback();
    virtual void ProcessBeackonCallback();
    bool readingSensore = false ;
    bool readingBeckon = false;
};

class ModbusSensorClient : public Node, public ModbusSensorClientInterface {
private:

    struct setSingleRegisterData{
        idData ID;
        short data;
    };

    ModbusClient* modbus;
    ModbusSensorClient();

    typedef map <idData, ModbusSensorClientInterface*> callbackRegisterMapType;
    typedef map <idData, ModbusSensorClientInterface*>::iterator it_type;
    callbackRegisterMapType callbackRegisterMap;

    mutex* m_mutex;
    bool shouldStop;
    int delayTime = 10;

    void main();

    friend bool operator< (const idData &id1, const idData &id2);

public:
    static ModbusSensorClient* instance;
    static ModbusSensorClient* getModbusSensorInstance();
    void stopModbusSensorClient();
    void registerToSensoreCallback(unsigned char _slave_address, unsigned short _coil_address, bool on_bool_callback, ModbusSensorClientInterface *obj);
    //void registerToModbusRegisterCallback()
};

} // end namespace modbus

#endif // _MODBUS_SENSORE_CLIENT
