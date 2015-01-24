#ifndef _MODBUSNOTIFICATION_H
#define _MODBUSNOTIFICATION_H

#include "messages/Notification.h"
#include "utils/helper/MotionState.h" //ako bude trebalo

using namespace robot;

namespace modbus {
class ModbusNotification: public Notification{
public:
    ModbusNotification();

};
}

#endif
