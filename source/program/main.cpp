#include "lib.hpp"

#include "nn.hpp"
#include "utils.hpp"
#include "structs.hpp"
#include "config.hpp"
#include "binaryoffsethelper.h"

#include <cstring>

WorldManagerModule** WorldMgrModuleInstancePtr = nullptr;
EventMgr** EventMgrInstancePtr = nullptr;
void** GameDataMgrInstancePtr = nullptr;
float timer = 0.0f;
bool just_voided = false;
constexpr u32 is_pause_hashes[] = { 0xf96989b6, 0x9a9506e7, 0x604aebe1, 0x3ce410c0, 0x0915eb45, 0xc2f987db, 0xbf5bf25f };
constexpr u32 oob_flag = 0x4b369531; // YouDoneFuckedUp
const char* voidout_event = "DmF_SY_FallDownReturn";
bool (*getBool)(void* mgr, bool& value, u32 hash);
void (*setBool)(void* mgr, bool value, u32 hash);

bool is_in_bounds(Vector3f& pos) {
    return pos.x <= MAX_X && pos.x >= MIN_X && pos.y <= MAX_Y && pos.y >= MIN_Y && pos.z <= MAX_Z && pos.z >= MIN_Z;
}

HOOK_DEFINE_TRAMPOLINE(GetPos) {
    static void Callback(AutoSaveMgr* mgr, void* arg) {
        
        char buf[0x100];

        bool is_paused;
        u8 i = 0;
        for (u32 hash : is_pause_hashes) {
            bool result = getBool(*GameDataMgrInstancePtr, is_paused, hash);
            if (!result) {
                PRINT("Failed to get flag");
                return Orig(mgr, arg);
            }
            if (is_paused) {
                if (i == 0 || i == 1) {
                    setBool(*GameDataMgrInstancePtr, false, oob_flag); // reset when warping
                    timer = 0.0f;
                    just_voided = false;
                }
                return Orig(mgr, arg);
            }
            ++i;
        }

        if ((*EventMgrInstancePtr)->mUnknownValue != 0) {
            return Orig(mgr, arg);
        }
        
        if (!mgr) {
            PRINT("AutoSaveMgr is null");
            return Orig(mgr, arg);
        }

        if (!is_in_bounds(mgr->mPosData.save_pos) ) {
            if (WorldMgrModuleInstancePtr) {
                if ((*WorldMgrModuleInstancePtr)->mWorldMgrCurrent == (*WorldMgrModuleInstancePtr)->mWorldMgrMainField) {
                    setBool(*GameDataMgrInstancePtr, true, oob_flag);
                }
            } else {
                PRINT("Could not determine WorldMgr");
            }
        }

        return Orig(mgr, arg);
    }
};

HOOK_DEFINE_TRAMPOLINE(GetPos2) {
    static void Callback(AutoSaveMgr2* mgr, void* arg) {

        char buf[0x100];

        bool is_paused;
        u8 i = 0;
        for (u32 hash : is_pause_hashes) {
            bool result = getBool(*GameDataMgrInstancePtr, is_paused, hash);
            if (!result) {
                PRINT("Failed to get flag");
                return Orig(mgr, arg);
            }
            if (is_paused) {
                if (i == 0 || i == 1) {
                    setBool(*GameDataMgrInstancePtr, false, oob_flag); // reset when warping
                    timer = 0.0f;
                    just_voided = false;
                }
                return Orig(mgr, arg);
            }
            ++i;
        }

        if ((*EventMgrInstancePtr)->mUnknownValue != 0) {
            return Orig(mgr, arg);
        }
        
        if (!mgr) {
            PRINT("AutoSaveMgr is null");
            return Orig(mgr, arg);
        }

        if (!is_in_bounds(mgr->mPosData.save_pos) ) {
            if (WorldMgrModuleInstancePtr) {
                if ((*WorldMgrModuleInstancePtr)->mWorldMgrCurrent == (*WorldMgrModuleInstancePtr)->mWorldMgrMainField) {
                    setBool(*GameDataMgrInstancePtr, true, oob_flag);
                }
            } else {
                PRINT("Could not determine WorldMgr");
            }
        }

        return Orig(mgr, arg);
    }
};

HOOK_DEFINE_INLINE(FixTrue) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        bool is_oob;
        bool result = getBool(*GameDataMgrInstancePtr, is_oob, oob_flag);
        char buf[0x40];
        if (!result) {
            PRINT("Failed to get bool flag");
            return;
        }
        if (is_oob && timer <= 0.0f) {
            ctx->W[8] = 1;
        }
    }
};

HOOK_DEFINE_INLINE(FixFalse) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        bool is_oob;
        bool result = getBool(*GameDataMgrInstancePtr, is_oob, oob_flag);
        char buf[0x40];
        if (!result) {
            PRINT("Failed to get bool flag");
            return;
        }
        if (is_oob && timer <= 0.0f) {
            ctx->W[8] = 0;
        }
    }
};

HOOK_DEFINE_INLINE(Print) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        char buf[0x10];
        PRINT("Here");
    }
};

HOOK_DEFINE_INLINE(SetTrue) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        bool is_oob;
        bool result = getBool(*GameDataMgrInstancePtr, is_oob, oob_flag);
        char buf[0x40];
        if (!result) {
            PRINT("Failed to get bool flag");
            return;
        }
        if (is_oob && timer <= 0.0f) {
            ctx->W[0] = 1;
            setBool(*GameDataMgrInstancePtr, false, oob_flag);
            just_voided = true;
        }
    }
};

HOOK_DEFINE_INLINE(StartTimer) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        const char* event_name = *(char**)(ctx->X[1]);
        if (strcmp(voidout_event, event_name) == 0 && just_voided) {
            timer = 90.0f;
            just_voided = false;
        }
    }
};

HOOK_DEFINE_INLINE(CalcTimer) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        if (timer > 0.0f) {
            float delta = *((float*)ctx->X[21]);
            char buf[0x20];
            timer -= delta;
            PRINT("Timer: %f", timer);
        }
    }
};

extern "C" void exl_main(void* x0, void* x1) {

    char buf[0x1000];

    /* Setup hooking enviroment */
    exl::hook::Initialize();
    const u64 main = exl::util::GetMainModuleInfo().m_Total.m_Start;

    /* Resolve version */
    PRINT("Getting app version...");
    int version = InitializeAppVersion();
    if (version == 0xffffffff || version > 5) {
        PRINT("Error getting version");
        return;
    }
    PRINT("Version index %d", version);

    WorldMgrModuleInstancePtr = (WorldManagerModule**)(main + wm_module_offset[version]);
    if (!WorldMgrModuleInstancePtr) {
        PRINT("Failed to get WorldManagerModule");
        return;
    }
    GameDataMgrInstancePtr = (void**)(main + gmd_mgr_offset[version]);
    if (!GameDataMgrInstancePtr) {
        PRINT("Failed to get GameDataMgr");
        return;
    }
    EventMgrInstancePtr = (EventMgr**)(main + event_mgr_offset[version]);
    if (!EventMgrInstancePtr) {
        PRINT("Failed to get EventMgr");
        return;
    }
    (*(uintptr_t*)&getBool) = (uintptr_t)(main + get_bool_offset[version]);
    if (!getBool) {
        PRINT("Failed to get gmd::GameDataMgr::getBool()");
        return;
    }
    (*(uintptr_t*)&setBool) = (uintptr_t)(main + set_bool_offset[version]);
    if (!setBool) {
        PRINT("Failed to get gmd::GameDataMgr::setBool()");
        return;
    }

    if (version > 1) {
        GetPos2::InstallAtOffset(calc_offset[version]);
    } else {
        GetPos::InstallAtOffset(calc_offset[version]);
    }
    FixFalse::InstallAtOffset(inline_hook_0[version]);
    SetTrue::InstallAtOffset(inline_hook_3[version]);
    CalcTimer::InstallAtOffset(inline_hook_4[version]);
    StartTimer::InstallAtOffset(inline_hook_5[version]);

    return;
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}