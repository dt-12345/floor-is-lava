#pragma once

#include "types.h"

constexpr float MAX_X = 1100.0f;
constexpr float MIN_X = -400.0f;
constexpr float MAX_Y = 2800.0f;
constexpr float MIN_Y = 1200.0f;
constexpr float MAX_Z = 2100.0f;
constexpr float MIN_Z = 400.0f;

// game::scene::AutoSaveMgr::calc()
static constexpr u64 calc_offset[] = {
    0x00c2e288,
    0x00c2f074,
    0x00c19d90,
    0x00c26e8c,
    0x00b76848,
    0x00c2cbd0
};

// engine::banc::WorldManagerModule::sInstance
static constexpr u64 wm_module_offset[] = {
    0x045f4ba8,
    0x046d06f8,
    0x046d8628,
    0x046caaa8,
    0x046bef00,
    0x046cdf78
};

// gmd::GameDataMgr::sInstance
constexpr u64 gmd_mgr_offset[] = {
    0x04646c60,
    0x047242b8,
    0x0472c1d8,
    0x0471e6b8,
    0x04712b18,
    0x04721b98
};

// engine::event::EventMgr::sInstance
constexpr u64 event_mgr_offset[] = {
    0x0464a988,
    0x04727fe8,
    0x0472ff08,
    0x047223e8,
    0x04716848,
    0x047258c8
};

// gmd::GameDataMgr::getBool()
static constexpr u64 get_bool_offset[] = {
    0x010886ec,
    0x0108e128,
    0x010a43f0,
    0x00be8834,
    0x00cd3c8c,
    0x00c2e034
};

// gmd::GameDataMgr::setBool()
static constexpr u64 set_bool_offset[] = {
    0x00a02aec,
    0x00ab7f84,
    0x00adc2c4,
    0x00ac1434,
    0x00aa2428,
    0x00b20a48
};

// game::component::PlayerComponent::postSensor()
// static constexpr u64 player_comp_post_sensor_offset[] = {
//     0x009632b8,
//     0x007eb780,
//     0x007393dc,
//     0x0081b924,
//     0x00950e28,
//     0x009e558c
// };

static constexpr u64 inline_hook_0[] = {
    0x00df7a70,
    0x00e22580,
    0x00e36d70,
    0x00e16330,
    0x00e4cca0,
    0x00e47d10
};

static constexpr u64 inline_hook_3[] = {
    0x00df7c24,
    0x00e22734,
    0x00e36f24,
    0x00e164e4,
    0x00e4ce38,
    0x00e47ea8
};

static constexpr u64 inline_hook_4[] = {
    0x00c2e754,
    0x00c2f540,
    0x00c1a240,
    0x00c2733c,
    0x00b76ea4,
    0x00c2d00c
};

static constexpr u64 inline_hook_5[] = {
    0x01d13d50,
    0x01d82be0,
    0x01d7d610,
    0x01d77540,
    0x01d695e8,
    0x01d75a88
};