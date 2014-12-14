#include "WebServer.h"

namespace robot {

void WebServer::main(){
    MyController myCnontroller;
    server.registerController(&myCnontroller);
    server.setOption("enable_directory_listing", "false");
    server.setOption("document_root", "web");

    server.start();
    {
        unique_lock<boost::mutex> lock(stopLock);
        while (!shouldStop) {
            stopRequest.wait(lock);
        }
        server.stop();
        debug("Stopping");
    }
}

void WebServer::stop(){
    stopLock.lock();
    shouldStop=true;
    stopLock.unlock();
    stopRequest.notify_all();
}

}
