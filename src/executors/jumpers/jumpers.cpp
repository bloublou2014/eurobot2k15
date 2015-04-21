#include "jumpers.h"

#define CROSS_COMPILING

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

    while(!shouldStop){
        if( digitalRead(side_jumper)){
            digitalWrite(led_side_GREEN, LOW);
            digitalWrite(led_side_YELLOW, HIGH);
            side = SideNotification::Side::GREEN;
        }else{
            digitalWrite(led_side_GREEN, HIGH);
            digitalWrite(led_side_YELLOW, LOW);
            side = SideNotification::Side::YELLOW;
        }

        if(!digitalRead(start_jumper)){

            // send notification
            SideNotification* sideNotification=new SideNotification(side);
            sendNotification(sideNotification);

            // send Start mssg
            StartMessage* mssg = new StartMessage("Milan");
            sendNotification((Notification*)mssg);

            digitalWrite(led_workiing_RED, HIGH);
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
