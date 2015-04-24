#ifndef ENEMYDETECTORCOMMAND_H
#define ENEMYDETECTORCOMMAND_H

#include "messages/Command.h"
#include "messages/CommandResponse.h"
#include "utils/helper/MotionState.h"

#include "drivers/motion/MotionDriver.h"

using namespace robot;
using geometry::Point2D;
using namespace v8;
using javascript::JavaScriptMessageProvider;
using javascript::ObjectWrap;

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
