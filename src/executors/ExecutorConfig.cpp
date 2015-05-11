#include "ExecutorConfig.h"



namespace executor {

ExecutorConfig::ExecutorConfig(){
    //printf("ExeutorConfig is called");
}

bool ExecutorConfig::reload(ConfigValues* values, string _executorName){
#ifdef VELIKI_ROBOT
    boost::property_tree::xml_parser::read_xml("Config.xml",pt);
#endif
#if MALI_ROBOT
    boost::property_tree::xml_parser::read_xml("Config.xml",pt);
#endif

    if(_executorName == "LiftLeftExecutor" || _executorName =="LiftRightExecutor"){
        //printf("reloadConfig: executorName is %s", _executorName);
        BOOST_FOREACH( ptree::value_type const& v, pt.get_child(_executorName)){
            if(v.first == "LieftLevelsConfig"){
                values->LiftConfigs.lift.level0 = v.second.get<short>("level0");
                values->LiftConfigs.lift.level1 = v.second.get<short>("level1");
                values->LiftConfigs.lift.level2 = v.second.get<short>("level2");
                values->LiftConfigs.lift.levelBall = v.second.get<short>("levelBall");
                //printf("newValue of level0 is %d", values->lift.level0);
            }else if(v.first == "DoorConfig"){
                values->LiftConfigs.door.openGetObject = v.second.get<short>("openGetObject");
                values->LiftConfigs.door.openLeaveObejct = v.second.get<short>("openLeaveObject");
                values->LiftConfigs.door.close = v.second.get<short>("close");
            }else if(v.first == "HandConfig"){
                values->LiftConfigs.hand.grab = v.second.get<short>("grab");
                values->LiftConfigs.hand.relese = v.second.get<short>("relese");
            }else if(v.first == "TimeConfig"){
                values->LiftConfigs.time.liftUp = v.second.get<int>("liftUp");
                values->LiftConfigs.time.liftDown = v.second.get<int>("liftDown");
                values->LiftConfigs.time.doorOpenClose = v.second.get<int>("doorOpenClose");
                values->LiftConfigs.time.handOpenClose = v.second.get<int>("handOpenClose");
            }
        }
        /*
        cout <<"******  RELOAD LIFT CONFIG **********" << endl;
        cout <<"Lift Level Config: " << endl;
        cout <<"level0: " << values->LiftConfigs.lift.level0 << endl;
        cout <<"level1: " << values->LiftConfigs.lift.level1 << endl;
        cout <<"level1: " << values->LiftConfigs.lift.level2 << endl;
        cout <<"Door Config: "<< endl;
        cout <<"openGetObject: "<< values->LiftConfigs.door.openGetObject << endl;
        cout <<"openLeaveObecjt: "<< values->LiftConfigs.door.openLeaveObejct << endl;
        cout <<"close: "<< values->LiftConfigs.door.close << endl;
        cout <<"Hand Config: " << endl;
        cout <<"open: "<< values->LiftConfigs.hand.relese << endl;
        cout <<"close: "<< values->LiftConfigs.hand.grab << endl;
        cout <<"Time config" << endl;
        cout << "liftUp : " << values->LiftConfigs.time.liftUp << endl;
        cout << "liftDown: " << values->LiftConfigs.time.liftDown << endl;
        cout << "doorOpenClose" << values->LiftConfigs.time.doorOpenClose << endl;
        cout << "handOpenClose" << values->LiftConfigs.time.handOpenClose << endl;
        cout << "********* END RELOAD LIFT CONIG **********" << endl;
        */
        // printf("ExeutorConfig is called");
    }else if(_executorName == "LiftCenterExecutor"){
        BOOST_FOREACH( ptree::value_type const& v, pt.get_child(_executorName)){
            if(v.first == "DoorConfig"){
                values->LiftConfigs.door.openGetObject = v.second.get<short>("openGetObject");
                values->LiftConfigs.door.close = v.second.get<short>("close");
            }else if(v.first == "HandConfig"){
                values->LiftConfigs.hand.grab = v.second.get<short>("grab");
                values->LiftConfigs.hand.relese = v.second.get<short>("relese");
            }
        }

    }else if(_executorName == "FlapExecutor"){

        BOOST_FOREACH( ptree::value_type const& v, pt.get_child(_executorName)){
            if(v.first == "FlapRightConfig"){
                values->FlapConfigs.flapRight.open = v.second.get<short>("open");
                values->FlapConfigs.flapRight.close = v.second.get<short>("close");
            }else if(v.first == "FlapLeftConfig"){
                values->FlapConfigs.flapLeft.open = v.second.get<short>("open");
                values->FlapConfigs.flapLeft.close = v.second.get<short>("close");
            }
        }
        /*
        cout << "******* RELOAD FLAP CONFIG **************" << endl;
        cout << "FlapRight Config:" << endl;
        cout << "open:" << values->FlapConfigs.flapRight.open << endl;
        cout << "close:" << values->FlapConfigs.flapRight.close << endl;
        cout << "FlapLeft Config:" << endl;
        cout << "open: " << values->FlapConfigs.flapLeft.open << endl;
        cout << "close: " << values->FlapConfigs.flapLeft.close << endl;
        cout << "********* END RELOAD FLAP CONIG **********" << endl;
        */
    }else if(_executorName == "PopcornExecutor"){
        BOOST_FOREACH( ptree::value_type const& v, pt.get_child(_executorName)){
            if(v.first == "PipeRightConfig"){
                values->PopcornConfigs.pipeRight.open = v.second.get<short>("open");
                values->PopcornConfigs.pipeRight.close = v.second.get<short>("close");
            }else if(v.first == "PipeLeftConfig"){
                values->PopcornConfigs.pipeLeft.open = v.second.get<short>("open");
                values->PopcornConfigs.pipeLeft.close = v.second.get<short>("close");
            }else if(v.first == "BackStorageConfig"){
                values->PopcornConfigs.backDoor.open = v.second.get<short>("open");
                values->PopcornConfigs.backDoor.close = v.second.get<short>("close");
            }else if(v.first == "TimeConfig"){
                values->PopcornConfigs.time.getPopcorn = v.second.get<int>("getPopcorn");
                values->PopcornConfigs.time.unloadPopcorn = v.second.get<int>("unloadPopcorn");
            }
        }
        /*
        cout << "***** RELOAD POPCORN CONFIG ***********" << endl;
        cout << "Pipe Right Config " << endl;
        cout << "open: " << values->PopcornConfigs.pipeRight.open << endl;
        cout << "close: " << values->PopcornConfigs.pipeRight.close << endl;
        cout << "Pipe Left Config: " << endl;
        cout << "open: " << values->PopcornConfigs.pipeLeft.open << endl;
        cout << "close: " << values->PopcornConfigs.pipeLeft.close << endl;
        cout << "Back Door Config: " << endl;
        cout << "open: " << values->PopcornConfigs.backDoor.open << endl;
        cout << "close: " << values->PopcornConfigs.backDoor.close << endl;
        cout << "Time Config" << endl;
        cout << "unloadPopcorn: " << values->PopcornConfigs.time.unloadPopcorn << endl;
        cout << "getPopcorn: " << values->PopcornConfigs.time.getPopcorn << endl;
        cout << "********* END POPCORN CONFIG ************" << endl;
        */
     }else if(_executorName == "CarpetExecutor"){
        BOOST_FOREACH( ptree::value_type const& v, pt.get_child(_executorName)){
            if(v.first == "armRightConfig"){
                values->CarpetConfig.armRight.positionOpen = v.second.get<short>("open");
                values->CarpetConfig.armRight.positionClose = v.second.get<short>("close");
                values->CarpetConfig.armRight.position1 = v.second.get<short>("position1");
                values->CarpetConfig.armRight.position2 = v.second.get<short>("position2");
            }else if(v.first == "armLeftConfig"){
                values->CarpetConfig.armLeft.positionOpen = v.second.get<short>("open");
                values->CarpetConfig.armLeft.positionClose = v.second.get<short>("close");
                values->CarpetConfig.armLeft.position1 = v.second.get<short>("position1");
                values->CarpetConfig.armLeft.position2 = v.second.get<short>("position2");
            }else if(v.first == "TimeConfig"){
                values->CarpetConfig.armOpenCloseTime  = v.second.get<int>("OpenArmTime");
                values->CarpetConfig.leaveCarpetTime = v.second.get<int>("LeaveCarpetTime");
            }
        }

    } //end if == executorName
    return true;
}

bool ExecutorConfig::reloadF(){
    printf("this function must be redefeined in executor");
    return false;
}

} // end executor namespace
