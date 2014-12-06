#ifndef _EXAMPLEEXECUTOR_H
#define _EXAMPLEEXECUTOR_H

#include "AbstractExecutor.h"

namespace robot{

class ExampleExecutor: public AbstractExecutor{
public:
    static string NAME;
    ExampleExecutor():AbstractExecutor(NAME){}

    void init();
    void processTestEvent(Notification* notification);
};

}

#endif //_EXAMPLEEXECUTOR_H
