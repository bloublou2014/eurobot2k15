#include "AbstractTask.h"

namespace robot{

TaskState AbstractTask::getTaskState() const{
    return state;
}

int AbstractTask::sendCommand(Command* command, responseCallback success, responseCallback error, responseCallback progress){
    command->setSource(getName());

    if (state==RUNNING){
        return CommandSource::sendCommand(command, success, error, progress);
    }else
        return -1;
}

bool AbstractTask::isSubscribed(Notification* message){
    return NotificationHandler::isSubscribed(message);
}

bool AbstractTask::passMessage(Message* message){
    queueLock.lock();
    instructionQueue.push(message);
    queueLock.unlock();
    queueNotEmpty.notify_one();
    return true;
}

bool AbstractTask::runTask(){
    queueLock.lock();
    instructionQueue.push(Instruction(Instruction::Type::START));
    queueLock.unlock();
    queueNotEmpty.notify_one();
    //TODO: mora se uraditi provera da li se task moze pokrenuti
    return true;
}

bool AbstractTask::pauseTask(){
    queueLock.lock();
    instructionQueue.push(Instruction(Instruction::Type::STOP));
    queueLock.unlock();
    queueNotEmpty.notify_one();
    //TODO: isto se mora proveriti uslov za pauziranje
    return true;
}

void AbstractTask::stop(){
    debug("Killing task");
    queueLock.lock();
    instructionQueue.push(Instruction(Instruction::Type::KILL));
    queueLock.unlock();
    queueNotEmpty.notify_one();
}

AbstractTask::Instruction AbstractTask::fetchInstruction(){
    unique_lock<boost::mutex> lock(queueLock);
    while (instructionQueue.empty()) {
        queueNotEmpty.wait(lock);
    }
    AbstractTask::Instruction instr=instructionQueue.front();
    instructionQueue.pop();
    return instr;
}

void AbstractTask::updateState(TaskState _state){
    handler->updateStatus(getName(),_state);
}

void AbstractTask::setState(TaskState _state){
    state=_state;
}

void AbstractTask::registerManager(TaskManagerInterface *manager){
    CommandSource::setHandler(manager);
    NotificationSource::setHandler(manager);
    handler=manager;
}

void AbstractTask::main(){
    setState(SUSPENDED);
    try{
        onCreate();

        while(!taskKilled){
            Instruction instr=fetchInstruction();
            if (instr.type==Instruction::Type::MESSAGE){
                Message* message=instr.message;
                switch (message->getMessageType()) {
                case NOTIFICATION:
                    processNotification((Notification*)message);
                    break;
                case COMMAND_RESPONSE:
                    processCommandResponse((CommandResponse*)message);
                    break;
                default:
                    break;
                }
            }else{
                switch(instr.type){
                case Instruction::Type::START:
                    setState(RUNNING);
                    onRun();
                    break;
                case Instruction::Type::STOP:
                    setState(READY);
                    onPause();
                    break;
                case Instruction::Type::KILL:
                    setState(IMPOSSIBLE);
                    taskKilled=true;
                    onDestroy();
                    break;
                default:
                    break;
                }
            }
        }
    }
    catch(TaskExecutionException& e){
        setState(IMPOSSIBLE);
        error("Task error. Reason:");
        error(e.what());
    }
    catch(std::exception& e){
        setState(IMPOSSIBLE);
        error("Error! something bad has happend.");
        error(e.what());
    }
}

}
