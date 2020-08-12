#pragma once

#include <list>
#include <map>
#include <unordered_set>

#include "Cmn/Def/Gear.h"

namespace gear {

static constexpr auto GEAR_SKILL_COUNT = 4;
static constexpr auto GEAR_SUB_SKILL_COUNT = 3;

enum class EditingSlot { MAIN, SUB0, SUB1, SUB2, ALL_SUB, ALL };

static constexpr auto MAIN_EXCLUSIVE_SKILL_ID_START = 100;

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

static constexpr auto SUB_LEGAL_SKILLS = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
};
static const auto SUB_LEGAL_SKILL_SET = std::unordered_set<decltype(Cmn::Def::Gear::mMainSkillId)>{SUB_LEGAL_SKILLS};

// inline auto skillIdIsValid(decltype(Cmn::Def::Gear::mMainSkillId) skillId, decltype(Cmn::Def::Gear::mGearId) gearId,
//                            Cmn::Def::GearKind gearKind) -> bool {
//     if (skillId < 0) return false;
//     if (skillId <= 13) return true;

//     switch (gearKind) {
//         case Cmn::Def::GearKind::cHead:
//             return 100 <= skillId and skillId <= 103;
//         case Cmn::Def::GearKind::cClothes:
//             return 104 <= skillId and skillId <= 108;
//         case Cmn::Def::GearKind::cShoes:
//             return 109 <= skillId and skillId <= 111;
//         default:
//             return false;
//     }
// }

using SkillList = std::list<decltype(Cmn::Def::Gear::mMainSkillId)>;

auto getGearSkills(decltype(Cmn::Def::Gear::mGearId) gearId, Cmn::Def::GearKind gearKind, EditingSlot editingSlot)
    -> SkillList;

}  // namespace gear
