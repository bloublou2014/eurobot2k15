#include "ExampleExecutor.h"

#include <iostream>

using namespace std;

namespace robot {

string ExampleExecutor::NAME="ExampleExecutor";

void ExampleExecutor::init(){
    this->registerCommand(CountdownCommand::NAME,(commandCallback)&ExampleExecutor::countdown);
}

void ExampleExecutor::countdown(Command* command){
    commandQueueLock.lock();
    commandsToProcess.push(Instruction(command));
    commandQueueLock.unlock();
    queueNotEmpty.notify_one();
}

void ExampleExecutor::stop(){
    commandQueueLock.lock();
    commandsToProcess.push(Instruction::STOP);
    commandQueueLock.unlock();
    queueNotEmpty.notify_one();
}

void ExampleExecutor::main(){
    shouldStop=false;
    while (true){
        if (shouldStop) break;

        Instruction instr=getNextInstruction();
        if (instr.type==Instruction::STOP){
            return;
        }

        CountdownCommand* cmd=(CountdownCommand*)instr.command;
        bool foundFinished=false;
        list<SleepTimer*>::iterator it=timers.begin();
        for(;it!=timers.end();++it){
            if ((*it)->isFinished()){
                (*it)->start(cmd->getCountdownValue(), cmd);
                foundFinished=true;
            }
        }
        if (!foundFinished){
            timers.push_back(new SleepTimer(this,cmd->getCountdownValue(), cmd));
        }
    }
    debug("Stopping execution");
}

void ExampleExecutor::onTimeout(const boost::system::error_code &e, void *obj){
    std::stringstream ss;
    CountdownCommand* cc=static_cast<CountdownCommand*>(obj);
    sendResponseFromCommand(cc);
}

ExampleExecutor::Instruction ExampleExecutor::getNextInstruction(){
    unique_lock<boost::mutex> lock(commandQueueLock);
    while (commandsToProcess.empty()) {
        queueNotEmpty.wait(lock);
    }
    Instruction instr=commandsToProcess.front();
    commandsToProcess.pop();
    return instr;
}

}
