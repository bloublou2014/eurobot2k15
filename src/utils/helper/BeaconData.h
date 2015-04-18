#ifndef _BEACON_DATA
#define _BEACON_DATA


namespace modbus{

struct beaconDataStruct{
    short X_beacon1;
    short Y_beacon1;
    short X_beacon2;
    short Y_beacon2;
};

struct enemyDetectedStruct{
    bool brkon;
    bool left;
    bool right;
    bool back;

};



}


#endif // _BEACON_DATA
