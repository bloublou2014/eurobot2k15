#ifndef JAVASCRIPTMESSAGEPROVIDER_H
#define JAVASCRIPTMESSAGEPROVIDER_H

#include "utils/javascript/JavaScriptMessage.h"

namespace javascript{

class JavaScriptMessageProvider
{
public:
    virtual JavaScriptMessage* getMessageHandler(const string& name)=0;
};

}

#endif // JAVASCRIPTMESSAGEPROVIDER_H
