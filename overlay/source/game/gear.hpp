#pragma once

#include <map>

#include "Cmn/Def/Gear.h"

namespace gear {

static constexpr auto GEAR_SKILL_COUNT = 26;
static const auto GEAR_SKILL_NAME_MAP = std::map<decltype(Cmn::Def::Gear::mMainSkillId), const char*>{
    {-2, "Blank"},    //
    {-1, "Unknown"},  //
    {0, "ISMn"},      // MainInk_Save
    {1, "ISSb"},      // SubInk_Save
    {2, "IRcU"},      // InkRecovery_Up
    {3, "RuSU"},      // HumanMove_Up
    {4, "SwSU"},      // SquidMove_Up
    {5, "SpCU"},      // SpecialIncrease_Up
    {6, "SpSv"},      // RespawnSpecialGauge_Save
    {7, "SpPU"},      // SpecialTime_Up
    {8, "QRsp"},      // RespawnTime_Save
    {9, "QSJp"},      // JumpTime_Save
    {10, "SbPU"},     // BombDistance_Up
    {11, "IRes"},     // OpInkEffect_Reduction
    {12, "BDfU"},     // BombDamage_Reduction
    {13, "MnPU"},     // MarkingTime_Reduction
    {100, "OpGb"},    // StartAllUp
    {101, "LDEf"},    // EndAllUp
    {102, "Tena"},    // MinorityUp
    {103, "Cbak"},    // ComeBack
    {104, "NjSq"},    // SquidMoveSpatter_Reduction
    {105, "Hunt"},    // DeathMarking
    {106, "ThmI"},    // ThermalInk
    {107, "RspP"},    // Exorcist
    {108, "AbDb"},    // ExSkillDouble
    {109, "StJp"},    // SuperJumpSign_Hide
    {110, "ObjS"},    // ObjectEffect_Up
    {111, "DRol"},    // SomersaultLanding
};

inline auto skillIdIsValid(decltype(Cmn::Def::Gear::mMainSkillId) skillId, decltype(Cmn::Def::Gear::mGearId) gearId,
                           Cmn::Def::GearKind gearKind) -> bool {
    // TODO: check for actual valid ids

    if (skillId < 0) return false;
    if (skillId <= 13) return true;

    switch (gearKind) {
        case Cmn::Def::GearKind::cHead:
            return 100 <= skillId and skillId <= 103;
        case Cmn::Def::GearKind::cClothes:
            return 104 <= skillId and skillId <= 108;
        case Cmn::Def::GearKind::cShoes:
            return 109 <= skillId and skillId <= 111;
        default:
            return false;
    }
}

}  // namespace gear
