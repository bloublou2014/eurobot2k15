#include "Scheduler.h"

namespace robot{

Scheduler::Scheduler(TaskManagerInterface *_hanler):Node("Scheduler"), started(false), shouldStop(false), activeTask(NULL), handler(_hanler){
}

bool Scheduler::passMessage(Message *message){
    queueLock.lock();
    instructionQueue.push(message);
    queueLock.unlock();
    queueNotEmpty.notify_one();
    return true;
}

bool Scheduler::isSubscribed(Notification *message){
    return NotificationHandler::isSubscribed(message);
}

void Scheduler::startDispatching(){
    queueLock.lock();
    instructionQueue.push(Instruction(Instruction::Type::START));
    queueLock.unlock();
    queueNotEmpty.notify_one();
}

void Scheduler::stop(){
    debug("Killing task");
    queueLock.lock();
    instructionQueue.push(Instruction(Instruction::Type::STOP));
    queueLock.unlock();
    queueNotEmpty.notify_one();
}

void Scheduler::updateTaskState(AbstractTask* task, TaskState oldState, TaskState newState){
    debug("Updating task state");
    Instruction instr(Instruction::Type::STATE_UPDATE);
    instr.oldState=oldState;
    instr.newState=newState;
    instr.task=task;
    queueLock.lock();
    instructionQueue.push(instr);
    queueLock.unlock();
    queueNotEmpty.notify_one();
//    debug("Updating task state");
}

Scheduler::Instruction Scheduler::fetchInstruction(){
    unique_lock<boost::mutex> lock(queueLock);
    while (instructionQueue.empty()) {
        queueNotEmpty.wait(lock);
    }
    Instruction instr=instructionQueue.front();
    instructionQueue.pop();
    return instr;
}

void Scheduler::main(){
    debug("Staring scheduler!");
    try{
        onCreate();

        while(!shouldStop){
            Instruction instr=fetchInstruction();
            if (instr.type==Instruction::Type::MESSAGE){
                Message* message=instr.message;
                switch (message->getMessageType()) {
                case NOTIFICATION:
                    processNotification((Notification*)message);
                    break;
                default:
                    break;
                }
            }else{
                switch(instr.type){
                case Instruction::Type::START:
                    started=true;
                    onStart();
                    break;
                case Instruction::Type::STOP:
                    started=false;
                    onStop();
                    shouldStop=true;
                    break;
                case Instruction::Type::STATE_UPDATE:
                    if (instr.oldState==TaskState::RUNNING && instr.newState!=TaskState::RUNNING){
                        setActiveTask(NULL);
                    }
                    onStateUpdate(instr.task, instr.oldState, instr.newState);
                    break;
                default:
                    break;
                }
            }
        }

    }catch(JavaScriptVMException& e){
        error("Javascript error. Reason:");
        error(e.what());
    }
    catch(std::exception& e){
        error("Error! something bad has happend.");
        error(e.what());
    }

}

AbstractTask *Scheduler::getActiveTask() const{
    return activeTask;
}

void Scheduler::setActiveTask(AbstractTask *task){
    activeTask=task;
}

bool Scheduler::hasStarted() const{
    return started;
}

TaskManagerInterface *Scheduler::getHandler() const{
    return handler;
}

void Scheduler::addTask(AbstractTask *task){
    availableTasks.push_back(task);
}

}
