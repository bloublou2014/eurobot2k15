#include "messages/Notification.h"

using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;
using namespace robot;

namespace executor{

class SideNotification : public Notification{
public:
    enum Side{
        YELLOW=1,
        GREEN=2
    };

    static string NAME;
    static void Init(Handle<Object> exports);
    //Getters
    static void SideGetter(Local<String> property, const PropertyCallbackInfo<Value>& info);

    SideNotification(Side _side):Notification(NAME,"JumperExecutor"),side(_side){}
    SideNotification(const SideNotification& ln):Notification(ln),side(ln.side){}
    Side getSide() const{ return side; }

    Message* clone(){
        return new SideNotification(*this);
    }

private:
    int side;
};

}
