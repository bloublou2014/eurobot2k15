#ifndef JSSCHEDULER_H
#define JSSCHEDULER_H

#include "core/Logger.h"
#include "core/NotificationHandler.h"
#include "core/JavaScriptVM.h"

namespace robot{

class JSScheduler:public NotificationHandler, public Logger, public JavaScriptVM{

public:
    JSScheduler();
};

}
#endif // JSSCHEDULER_H
