#include "ExecutorConfig.h"



namespace executor {

ExecutorConfig::ExecutorConfig(){
    //printf("ExeutorConfig is called");
}

bool ExecutorConfig::reload(ConfigValues* values, string _executorName){

    boost::property_tree::xml_parser::read_xml("Config.xml",pt);
    if(_executorName == "LiftLeftExecutor" || _executorName =="LiftRightExecutor"){
        //printf("reloadConfig: executorName is %s", _executorName);
        BOOST_FOREACH( ptree::value_type const& v, pt.get_child(_executorName)){
            if(v.first == "LieftLevelsConfig"){
                values->LiftConfigs.lift.level0 = v.second.get<short>("level0");
                values->LiftConfigs.lift.level1 = v.second.get<short>("level1");
                values->LiftConfigs.lift.level2 = v.second.get<short>("level2");
                //printf("newValue of level0 is %d", values->lift.level0);
            }else if(v.first == "DoorConfig"){
                values->LiftConfigs.door.openGetObject = v.second.get<short>("openGetObject");
                values->LiftConfigs.door.openLeaveObejct = v.second.get<short>("openLeaveObject");
                values->LiftConfigs.door.close = v.second.get<short>("close");
            }else if(v.first == "HandConfig"){
                values->LiftConfigs.hand.grab = v.second.get<short>("grab");
                values->LiftConfigs.hand.relese = v.second.get<short>("relese");
            }else if(v.first == "TimeConfig"){
                values->LiftConfigs.time.adjust_position = v.second.get<int>("adjust_position");
                values->LiftConfigs.time.interval = v.second.get<int>("interval");
                values->LiftConfigs.time.some_time = v.second.get<int>("some_time");
            }
        }
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
        cout << "adjust Position: " << values->LiftConfigs.time.adjust_position;
        cout << "interval: " << values->LiftConfigs.time.adjust_position << endl;
        cout << "some time: " << values->LiftConfigs.time.some_time << endl;
        cout << "********* END RELOAD LIFT CONIG **********" << endl;

        // printf("ExeutorConfig is called");
    }else if(_executorName == "LiftCenterExecutor"){
        BOOST_FOREACH( ptree::value_type const& v, pt.get_child(_executorName)){
            if(v.first == "DoorConfig"){
                values->LiftConfigs.door.openGetObject = v.second.get<short>("openGetObject");
                values->LiftConfigs.door.close = v.second.get<short>("close");
            }else if(v.first == "HandConfig"){
                values->LiftConfigs.hand.grab = v.second.get<short>("grab");
                values->LiftConfigs.hand.relese = v.second.get<short>("relese");
            }else if(v.first == "TimeConfig"){
                values->LiftConfigs.time.adjust_position = v.second.get<int>("adjust_position");
                values->LiftConfigs.time.interval = v.second.get<int>("interval");
                values->LiftConfigs.time.some_time = v.second.get<int>("some_time");
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
            }else if(v.first == "TimeConfig"){
                values->FlapConfigs.time.adjust_position = v.second.get<int>("adjust_position");
                values->FlapConfigs.time.interval = v.second.get<int>("interval");
                values->FlapConfigs.time.some_time = v.second.get<int>("some_time");
            }
        }
        cout << "******* RELOAD FLAP CONFIG **************" << endl;
        cout << "FlapRight Config:" << endl;
        cout << "open:" << values->FlapConfigs.flapRight.open << endl;
        cout << "close:" << values->FlapConfigs.flapRight.close << endl;
        cout << "FlapLeft Config:" << endl;
        cout << "open: " << values->FlapConfigs.flapLeft.open << endl;
        cout << "close: " << values->FlapConfigs.flapLeft.close << endl;
        cout <<"Time config" << endl;
        cout << "adjust Position: " << values->FlapConfigs.time.adjust_position << endl;
        cout << "interval: " << values->FlapConfigs.time.adjust_position << endl;
        cout << "some time: " << values->FlapConfigs.time.some_time << endl;
        cout << "********* END RELOAD FLAP CONIG **********" << endl;

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
                values->PopcornConfigs.time.adjust_position = v.second.get<int>("adjust_position");
                values->PopcornConfigs.time.interval = v.second.get<int>("interval");
                values->PopcornConfigs.time.some_time = v.second.get<int>("some_time");
            }
        }
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
        cout << "********* END POPCORN CONFIG ************" << endl;
     }
    return true;
}

bool ExecutorConfig::reloadF(){
    printf("this function must be redefeined in executor");
    return false;
}

} // end executor namespace
