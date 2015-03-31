#include "PipeTask.h"

namespace robot{

void PipeTask::initScript(){
    this->subscribe("milan",(notificationCallback)&PipeTask::readLine);
    executorMap["LiftLeft"] = LIFT_LEFT; // nesto sam ujeobao kod ovoga = mrzelo da gledam presao na if :D
    executorMap["LiftRight"] = LIFT_RIGHT;
    executorMap["LiftCenter"] = LIFT_CENTER;
    executorMap["Popcorn"] = POPCORN;
    executorMap["Flap"] = FLAP;

    actionMap["UnloadObject"] = UNLOAD_OBJECT;
    actionMap["UnloadPopcorn"] = UNLOAD_POPCORN;
    //actionMap["Kick"] = KICK;
    actionMap["GetObject"] = GET_OBJECT;
    actionMap["GetPopcorn"] = GET_POPCORN;
    actionMap["ReloadConfig"] = RELOAD_CONFIG;


}

void PipeTask::stopScript(){
    shouldStop = true;
    debug("sholudStop = true");
}

void PipeTask::init(){
    // nop nop nop

}

void PipeTask::startScript(){

}

void PipeTask::readLine(Notification *testNotification){

    debug("recived Milan notification and starting reading from fifo");
    while(!shouldStop){

        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        /*
        fifoLock.lock();
        fifo.open("cmdFifo", ifstream::in);
        std::getline(fifo,line);
        fifoLock.unlock();
        debug("line is ");
        std::cout << line << std::endl;

        if( line.compare("exit") == 0 ) {
            fifoLock.lock();
            fifo.close();
            fifoLock.unlock();
            break;
        }
        */
        /*
        if ( line.compare("tome") == 0 ){
            debug("compare correct sending command");
            GetObject* cmd6 = (GetObject*) ActuatorAction::LiftRight(ActuatorCommandType::ActuatorType::GET_OBJECT);
            sendCommand(cmd6 ,(responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
        }
        */




        switch(state){

        case READ_PIPE:{

            fifoLock.lock();
            fifo.open("cmdFifo", ifstream::in);
            std::getline(fifo,line);
            fifoLock.unlock();
            //debug("line is ");
            //std::cout << line << std::endl;
            fifoLock.lock();
            fifo.close();
            fifoLock.unlock();


            if( line.compare("exit") == 0 ) {
                debug("EXIT");
                fifoLock.lock();
                fifo.close();
                fifoLock.unlock();
                shouldStop = true; break;
            }


            istringstream string_stream(line);



            while(string_stream >> word){

                cout<< "word:" << word << std::endl;
                queueLock.lock();
                wordQueue.push(word);
                queueLock.unlock();
            }

            state = READ_WORD;
            break;
        }

        case READ_WORD:{
            if (wordQueue.empty()){
                debug("missing argument");
                state = READ_PIPE;
                break;
            }
            queueLock.lock();
            line =wordQueue.front();
            wordQueue.pop();
            queueLock.unlock();

            //line = *helper;
            cout << "line is" << line << endl;
            state = returnState;
            break;
        }

        case EXEC_NAME:{
            //            executor = executorMap[line];
            if (line.compare("LiftLeft") == 0)  executor = LIFT_LEFT;
            else if (line.compare("LiftRight") == 0) executor = LIFT_RIGHT;
            else if (line.compare("LiftCenter") == 0) executor = LIFT_CENTER;
            else if (line.compare("Popcorn") == 0) executor = POPCORN;
            else if (line.compare("Flap")  == 0) executor = FLAP;
            else if (line.compare("Motion") == 0) executor = MOTION;
            else { debug("error,no such executor, try again ");state = CLEAR_PIPE; returnState = EXEC_NAME;  break ;  }

            state = READ_WORD;
            returnState = EXEC_OPTION;
            debug("exec_name");
            //std::cout << executor << std::endl;
            break;
        }
        case EXEC_OPTION:{
            if (executor == LIFT_CENTER || executor  == LIFT_LEFT || executor == LIFT_RIGHT) {
                // action = actionMap[line];
                debug("liftExecutor");
                state = EXEC_SEND;
                if(line.compare("GetObject") == 0) action = GET_OBJECT;
                else if (line.compare("UnloadObject") == 0) action = UNLOAD_OBJECT;
                else if (line.compare("ReloadConfig") == 0) action = RELOAD_CONFIG;
                else { debug("ERROR: lift option, no such action for executor"); state = CLEAR_PIPE; returnState = EXEC_OPTION;  break;  }

            }else if  (executor == FLAP){
                debug("flap executor");
                state = EXEC_SEND;
                //action = actionMap[line];
                if (line.compare("KickRight") == 0) action = KICK_RIGHT;
                else if (line.compare("KickLeft") == 0) action = KICK_LEFT;
                else if (line.compare("UnKickRight") == 0) action = UNKICK_RIGHT;
                else if (line.compare("UnKickLeft") == 0) action = UNKICK_LEFT;
                else if (line.compare("ReloadConfig") == 0 ) action = RELOAD_CONFIG;
                else { debug("ERROR: flap executor, no such action for executor"), state = CLEAR_PIPE; returnState = EXEC_OPTION; break; }

            }else if (executor == POPCORN){
                //action = actionMap[line];
                debug("Popcorn executor");
                state = EXEC_SEND;
                if( line.compare("UnloadPopcorn") == 0) action = UNLOAD_POPCORN;
                else if ( line.compare("GetPopcorn") == 0) action = GET_POPCORN;
                else if (line.compare("ReloadConfig") == 0) action = RELOAD_CONFIG;
                else { debug("ERROR: Popcorn executor, no such action for executor"); state = CLEAR_PIPE; returnState = EXEC_OPTION; break;  }
            }else{
                debug("Motion executor");
                state = READ_WORD;
                returnState = EXEC_ARGV0;
                if(line.compare("MoveStraight") == 0) motionAction = MotionCommand::MotionType::MOVE_STRAIGHT;
                else if(line.compare("MoveToPosition") == 0) motionAction = MotionCommand::MotionType::MOVE_TO_POSITION;
                else if(line.compare("RotateFor") == 0) motionAction = MotionCommand::MotionType::ROTATE_FOR;
                else if(line.compare("RotateTo") == 0) motionAction = MotionCommand::MotionType::ROTATE_TO;
                else if(line.compare("Stop") == 0) motionAction = MotionCommand::MotionType::STOP;
                else if(line.compare("SetSpeed") == 0) motionAction = MotionCommand::MotionType::SET_SPEED;
                else { debug("ERROR: Motion executor, no such action for executor"); state = CLEAR_PIPE; returnState = EXEC_OPTION; break; }
            }
            break;
        }
        case EXEC_ARGV0:{
            argv = std::stoi(line);
            std::cout << argv << std::endl;
            state = EXEC_SEND;
            if (motionAction == MotionCommand::MotionType::MOVE_TO_POSITION){
                state = READ_WORD;
                returnState = EXEC_ARGV1;
            }
            break;
        }
        case EXEC_ARGV1:{
            argv1 = std::stoi(line);
            std::cout << argv1 << std::endl;
            state = EXEC_SEND;
            break;
        }
        case EXEC_SEND:{
            state = CLEAR_PIPE;
            returnState = EXEC_NAME;
            debug("SENDING COMMAND");
            switch (executor){
            case LIFT_LEFT:{
                sendCommand(ActuatorAction::LiftLeft(action),(responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
                break;
            }
            case LIFT_RIGHT:{
                sendCommand(ActuatorAction::LiftRight(action),(responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
                break;
            }
            case LIFT_CENTER:{
                sendCommand(ActuatorAction::LiftCenter(action),(responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
                break;
            }
            case POPCORN:{
                sendCommand(ActuatorAction::Popcorn(action),(responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
                break;
            }
            case FLAP:{
                sendCommand(ActuatorAction::Flap(action),(responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
                break;
            }
            case MOTION:{
                switch(motionAction){
                case MotionCommand::MotionType::MOVE_STRAIGHT:{
                    debug("MoveStraight");
                    sendCommand( new MoveForward(argv),(responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
                    break;
                }
                case MotionCommand::MotionType::MOVE_TO_POSITION:{
                    debug("MoveToPosition");
                    sendCommand( new MoveToPosition(Point2D(argv, argv)), (responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
                    break;
                }
                case MotionCommand::MotionType::ROTATE_FOR:{
                    debug("RotateFor");
                    sendCommand(new RotateFor(argv),(responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
                    break;
                }
                case MotionCommand::MotionType::SET_SPEED:{
                    debug("SetSpeed");
                    sendCommand(new SetSpeedMotion(argv),(responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
                    break;
                }
                default:{
                    debug("TODO");
                    break;
                }
                }

                //sendCommand( new MoveForward(argv),(responseCallback)&PipeTask::commandResponseSuccess, (responseCallback)&PipeTask::commandResponseError );
            }
            }

            break;
        }
        case CLEAR_PIPE:{
            while(!wordQueue.empty()) wordQueue.pop();
            state = READ_PIPE;
            returnState = EXEC_NAME;
            break;
        }
        } // end switch


    }// main while loop
}

void PipeTask::commandResponseError(Command *_command){
    debug("error");
}

void PipeTask::commandResponseSuccess(Command *_command){
    debug("success");
}

} // end namespace
