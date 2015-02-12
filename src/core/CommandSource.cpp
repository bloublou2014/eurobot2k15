#include "CommandSource.h"


namespace robot{

int CommandSource::sendCommand(Command* command, responseCallback success, responseCallback error, responseCallback progress){
    command->setId(++lastCommandId);
    callbackObjects.push_back(CallbackObject(lastCommandId,success, error, progress));
    handler->receiveMessage(command);
    return command->getId();
}

void CommandSource::setHandler(AbstractMessageHandler* _handler){
    handler=_handler;
}

CommandSource::CallbackObject::CallbackObject(int _id,responseCallback _success,responseCallback _error,responseCallback _progress):
    commandId(_id),success(_success),error(_error),progress(_progress){}

void CommandSource::processCommandResponse(CommandResponse* response){
    int id=Command::INVALID_ID;
    if((id=response->getId())!=Command::INVALID_ID){
        for (list<CommandSource::CallbackObject>::iterator it=callbackObjects.begin();it!=callbackObjects.end();++it){
            if (it->commandId==id){
                if ((response->getStatus()==ResponseStatus::SUCCESS)&&(it->success!=NULL)){
                    callbackObjects.erase(it);
                    (this->*it->success)(response);
                }
                if ((response->getStatus()==ResponseStatus::ERROR)&&(it->error!=NULL)){
                    callbackObjects.erase(it);
                    (this->*it->error)(response);
                }
                if ((response->getStatus()==ResponseStatus::PROGRESS_UPDATE)&&(it->progress!=NULL))
                    (this->*it->progress)(response);
                return;
            }
        }
    }
}

}

