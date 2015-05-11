#include "jumpers.h"

//#define CROSS_COMPILING

namespace executor{
#ifdef CROSS_COMPILING

string JumpersExecutor::NAME = "JumpersExecutor";

void JumpersExecutor::init(){
    wiringPiSetupGpio();
    pinMode(start_jumper, INPUT);
    pinMode(side_jumper, INPUT);
    pinMode(led_side_GREEN, OUTPUT);
    pinMode(led_side_YELLOW, OUTPUT);
    pinMode(led_workiing_RED, OUTPUT);
}

void JumpersExecutor::main(){
    while(!digitalRead(start_jumper)){
        digitalWrite(led_workiing_RED, LOW);
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
        digitalWrite(led_workiing_RED, HIGH);
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }
    while(!shouldStop){
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        if( digitalRead(side_jumper)){
            digitalWrite(led_side_GREEN, LOW);
            digitalWrite(led_side_YELLOW, HIGH);
            color = StartMessage::Color::GREEN;
        }else{
            digitalWrite(led_side_GREEN, HIGH);
            digitalWrite(led_side_YELLOW, LOW);
            color = StartMessage::Color::YELLOW;
        }

        if(!digitalRead(start_jumper)){
            debug("*********************START MSSG**************************");
            digitalWrite(led_workiing_RED, HIGH);

            // send Start mssg
            StartMessage* mssg = new StartMessage(color, "Milan");
            sendRawMessage(mssg);
            shouldStop = true;
        }else{
            digitalWrite(led_workiing_RED, LOW);
        }

    }
}

void JumpersExecutor::stop(){
    shouldStop = true;
}

#endif

} // end namespace
