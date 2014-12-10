#include "WebServer.h"

namespace robot {
void WebServer::start(){
    thread=boost::thread(&WebServer::main,this);
}

void WebServer::main(){
    server.setOption("enable_directory_listing", "false");
    server.setOption("document_root", "web");

    server.start();
    {
        unique_lock<boost::mutex> lock(stopLock);
        while (!shouldStop) {
            stopRequest.wait(lock);
        }
        server.stop();
    }
}

void WebServer::stop(){
    stopLock.lock();
    shouldStop=true;
    stopLock.unlock();
    stopRequest.notify_all();
}

}
