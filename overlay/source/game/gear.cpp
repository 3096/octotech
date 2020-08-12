#include "gear.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace gear {

auto getGearSkills(decltype(Cmn::Def::Gear::mGearId) gearId, Cmn::Def::GearKind gearKind, EditingSlot editingSlot)
    -> SkillList {
    if (editingSlot != EditingSlot::MAIN and editingSlot != EditingSlot::ALL) {
        return SUB_LEGAL_SKILLS;
    }

    // TODO: check for grizzco gear range
    auto skillFilePath = std::filesystem::path{"romfs:/valid_skill_ids/" + std::to_string(static_cast<int>(gearKind)) +
                                               "/" + std::to_string(gearId) + ".bin"};
    if (not std::filesystem::exists(skillFilePath)) {
        // TODO: hook the game's gear default skill function when not available
        return {};
    }

    auto skillFileIfstream = std::ifstream(skillFilePath, std::ifstream::binary | std::ifstream::in);
    auto skillCount = uint32_t{};
    skillFileIfstream.read(reinterpret_cast<char*>(&skillCount), sizeof(skillCount));

    auto result = SkillList{};
    for (auto i = 0u; i < skillCount; i++) {
        auto curSkillId = decltype(Cmn::Def::Gear::mMainSkillId){};
        skillFileIfstream.read(reinterpret_cast<char*>(&curSkillId), sizeof(curSkillId));

        if (curSkillId >= MAIN_EXCLUSIVE_SKILL_ID_START and editingSlot != EditingSlot::MAIN) {
            break;
        }
        result.push_back(curSkillId);
    }

    return result;
}

}  // namespace gear
