#ifndef _RELOADEXECUTORCONFIG_H
#define _RELOADEXECUTORCONFIG_H

//#include "AbstractLiftExecutor.h"

#include "boost/algorithm/string.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/foreach.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using boost::property_tree::ptree;
using namespace boost::property_tree::xml_parser;

namespace executor{

class ExecutorConfig{

private:
    struct LieftLevelsConfig{
        short level0; // kada smo tek iznad podloge da nosimo sve sto imamo
        short level1;
        short level2;
        short levelBall;
    };

    struct OpenCloseConfig{
        short open;
        short close;
    };

    struct HandConfig{
        short grab;
        short relese;
    };

    struct DoorConfig{
        short openGetObject;
        short openLeaveObejct;
        short close;
    };

    struct TimeConfig{
        int interval;
        int adjust_position;
        int some_time;
    };

    struct TimeConfigLift{
        int liftUp;
        int liftDown;
        int doorOpenClose;
        int handOpenClose;
    };

    struct TimeConfigPopcorn{
        int getPopcorn;
        int unloadPopcorn;
    };
    struct CarpetArmConfig{
        short positionClose;
        short positionOpen;
        short position1;
        short position2;
    };

    ptree pt;


protected:
    struct LiftExecutorConfig {
        LieftLevelsConfig lift;
        DoorConfig door;
        HandConfig hand;
        TimeConfigLift time;
    };

    struct PopcornExecutorConfigs{
        OpenCloseConfig pipeRight;
        OpenCloseConfig backDoor;
        OpenCloseConfig pipeLeft;
        TimeConfigPopcorn time;
    };

    struct FlapExecutorConfigs{
        OpenCloseConfig flapRight;
        OpenCloseConfig flapLeft;
    };

    struct CarpetExecutorConfigs{
        CarpetArmConfig armLeft;
        CarpetArmConfig armRight;
        int armOpenCloseTime;
        int leaveCarpetTime;
    };

    struct ConfigValues{   // TODO UNION
        LiftExecutorConfig LiftConfigs;
        PopcornExecutorConfigs PopcornConfigs;
        FlapExecutorConfigs FlapConfigs;
        CarpetExecutorConfigs CarpetConfig;
    };


public:
    ExecutorConfig();
    virtual bool reloadF();
    bool reload(ConfigValues* values, string _executorName);
};

}


#endif // _RELOADEXECUTORCONFIG_H
