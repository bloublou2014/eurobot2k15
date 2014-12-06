#include "CommandSource.h"


namespace robot{

bool CommandSource::sendCommand(Command* command, responseCallback success, responseCallback error, responseCallback progress){
    command->setId(++lastCommandId);
    callbackObjects.push_back(CallbackObject(lastCommandId,success, error, progress));

    //TODO: send command to core
}

CommandSource::CallbackObject::CallbackObject(int _id,responseCallback _success,responseCallback _error,responseCallback _progress):
    commandId(_id),success(_success),error(_error),progress(_progress){}

void CommandSource::processCommandResponse(CommandResponse* response){
    int id=Command::INVALID_ID;
    if((id=response->getId())!=Command::INVALID_ID){
        for (list<CommandSource::CallbackObject>::iterator it=callbackObjects.begin();it!=callbackObjects.end();++it){
            if (it->commandId==id){
                if ((response->getStatus()==ResponseStatus::SUCCESS)&&(it->success!=NULL))
                    (this->*it->success)(response);
                if ((response->getStatus()==ResponseStatus::ERROR)&&(it->error!=NULL))
                    (this->*it->error)(response);
                if ((response->getStatus()==ResponseStatus::PROGRESS_UPDATE)&&(it->progress!=NULL))
                    (this->*it->progress)(response);
                callbackObjects.erase(it);
                return;
            }
        }
    }
}

}

