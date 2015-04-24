#ifndef JAVASCRIPTMESSAGEFACTORY_H
#define JAVASCRIPTMESSAGEFACTORY_H

#include <string>
#include <map>
#include <src/v8.h>
#include "utils/javascript/JavaScriptMessageProvider.h"

#include "executors/msg/CountdownCommand.h"
#include "executors/msg/TimePassed.h"
#include "executors/msg/MotionCommand.h"
#include "executors/msg/MotionNotification.h"
#include "executors/msg/GetMotionState.h"
#include "messages/CommandResponse.h"
#include "executors/msg/ActuatorCommand.h"
#include "executors/msg/LIftNotification.h"
#include "executors/msg/EnemyDetectedNotification.h"
#include "executors/msg/SideNotification.h"
#include "core/TaskStateChangeNotification.h"
#include "executors/msg/EnemyDetectorCommand.h"

using std::string;
using std::map;
using namespace v8;

using robot::CountdownCommand;
using robot::TimePassedNotification;
using robot::Notification;

using executor::ActuatorCommandJS;

namespace javascript{

class JavaScriptMessageFactory : public JavaScriptMessageProvider
{
public:
    JavaScriptMessageFactory(){}

    void init(Handle<Object> exportObject);

    Handle<Function> getObjectConstructor(const string& name);
    void setObjectConstructor(const string &name, Handle<Function> newTemplate);

    bool hasObject(const string& objName) const;
    Handle<Object> wrapObject(const string& name, Isolate* isolate, robot::Message *ptr);
    void setObjectTemplate(const string& name, Handle<ObjectTemplate> tmpl);
private:
    map<string, Persistent<Function>> registeredConstructors;
    map<string, Persistent<ObjectTemplate>> registeredObjectTemplates;
};

}
#endif // JAVASCRIPTMESSAGEFACTORY_H
