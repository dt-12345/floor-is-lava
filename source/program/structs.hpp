#pragma once

#include "types.h"

struct Vector3f {
    float x;
    float y;
    float z;
};

struct PosStruct {
    u8 pad[0xc];
    Vector3f save_pos;
};

// for ver 1.0.0 and 1.1.0
struct AutoSaveMgr {
    u8 pad[0x160];
    PosStruct mPosData;
};

// for ver 1.1.1+
struct AutoSaveMgr2 {
    u8 pad[0x168];
    PosStruct mPosData;
};

struct WorldManagerModule {
    u8 pad[0xb0];
    void* mWorldMgrNone;
    void* mWorldMgrMainField;
    void* mWorldMgrCDungeon;
    void* mWorldMgrOpeningField;
    void* mWorldMgrCurrent;
};

struct EventMgr {
    u8 pad[0x58];
    u64 mUnknownValue; // probably whether or not there's an event active but too lazy to check
};