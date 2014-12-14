#include "AbstractTask.h"

namespace robot{

TaskState AbstractTask::getTaskState() const{
    return state;
}

bool AbstractTask::sendCommand(Command* command, responseCallback success, responseCallback error, responseCallback progress){
    //if (state==RUNNING){
        return CommandSource::sendCommand(command, success, error, progress);
   // }else
        //return false;
}

bool AbstractTask::passMessage(Message* message){
    queueLock.lock();
    instructionQueue.push(message);
    queueLock.unlock();
    queueNotEmpty.notify_one();
}

void AbstractTask::startTask(){
    queueLock.lock();
    instructionQueue.push(Instruction(Instruction::Type::START));
    queueLock.unlock();
    queueNotEmpty.notify_one();
}

void AbstractTask::stopTask(){
    queueLock.lock();
    instructionQueue.push(Instruction(Instruction::Type::STOP));
    queueLock.unlock();
    queueNotEmpty.notify_one();
}

void AbstractTask::killTask(){
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

void AbstractTask::setState(TaskState _state){
    state=_state;
}

void AbstractTask::registerManager(AbstractMessageHandler* manager){
    CommandSource::setHandler(manager);
    NotificationSource::setHandler(manager);
}

void AbstractTask::main(){
    setState(SUSPENDED);
    initScript();

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
                //ovde treba updateovati heap u manageru
                startScript();
                break;
            case Instruction::Type::STOP:
                setState(READY);
                //ovde treba updateovati heap u manageru
                stopScript();
                break;
            case Instruction::Type::KILL:
                setState(IMPOSSIBLE);
                //ovde treba updateovati heap u manageru
                taskKilled=true;
                break;
            default:
                break;
            }
        }
    }
}

}
