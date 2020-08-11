#include "gear_screen.hpp"

#include <cstddef>

#include "../game/gear.hpp"
#include "../game/save.hpp"
#include "../theme.hpp"
#include "skill_screen.hpp"

GearScreen GearScreen::s_instance;

GearScreen::GearScreen() : LOGCONSTRUCT m_basicScreen(*this) {
    lv_obj_t* p_window = lx::ui::lv_win::create(getLvScreenObj(), Theme::getThemeColorBgStyle());
    m_basicScreen.addLvObjPositionUpdater(p_window, lx::ui::lv_win::updateFitParent);
    lv_win_set_title(p_window, "  Gear Edit");

    for (auto gearKind : DISPLAY_GEAR_KINDS) {
        auto p_label = lx::ui::lv_label::create(p_window);
        m_basicScreen.addLvObjPositionUpdater(p_label, [p_window, gearKind](lv_obj_t* p_displayLabel) {
            lv_obj_align(p_displayLabel, p_window, LV_ALIGN_IN_TOP_LEFT, lx::ui::size::MARGIN(),
                         getDisplayGearY_(gearKind));
        });
        lv_label_set_text(p_label, GEAR_KIND_LABEL_STRS[gearKind]);
        m_displayGearLabels[gearKind] = p_label;

        auto p_btnMatrix = lx::ui::lv_btnmatrix::create(p_window);
        m_basicScreen.addLvObjPositionUpdater(p_btnMatrix, [p_window, gearKind](lv_obj_t* p_btnMatrix) {
            lv_obj_set_size(p_btnMatrix, lx::ui::size::getScreenWidth() - lx::ui::size::MARGIN() * 2,
                            lx::ui::size::coord(GEAR_SKILL_BUTTON_HEIGHT));
            lv_obj_align(p_btnMatrix, p_window, LV_ALIGN_IN_TOP_MID, 0,
                         getDisplayGearY_(gearKind) + lx::ui::size::coord(GEAR_SKILL_BUTTON_SINK));
        });
        m_displayGearButtons[gearKind] = p_btnMatrix;
        lv_group_add_obj(m_basicScreen.getLvInputGroup(), p_btnMatrix);
        lv_obj_set_event_cb(p_btnMatrix, handleGearSkillButtonClick_);
    }

    LOG("done");
}

GearScreen::~GearScreen() {}

const lx::LvKeyMap GearScreen::LV_KEY_MAP = {
    {KEY_DUP, LV_KEY_PREV},   {KEY_DDOWN, LV_KEY_NEXT}, {KEY_DRIGHT, LV_KEY_RIGHT},
    {KEY_DLEFT, LV_KEY_LEFT}, {KEY_ZR, LV_KEY_ENTER},
};

void GearScreen::procFrame() {
    if (MemoryReader::gameIsRunning()) {
        auto saveDataCmnDataBuffer = save::getSaveDataCmnData();
        auto& saveDataCmnData = *reinterpret_cast<Cmn::SaveDataCmn::SaveDataCmnData*>(saveDataCmnDataBuffer.get());

        auto curEquippedGearIds = EquippedGearIds{saveDataCmnData.equippedHeadGearId,
                                                  saveDataCmnData.equippedClothesId, saveDataCmnData.equippedShoesId};

        for (auto gearKind : DISPLAY_GEAR_KINDS) {
            if (m_lastEquippedGearIds[gearKind] == curEquippedGearIds[gearKind]) {
                continue;
            }

            Cmn::SaveDataCmn::SaveDataCmnData::GearKindInventory* p_curGearInventory = nullptr;
            switch (gearKind) {
                case HEAD_GEAR:
                    p_curGearInventory = &saveDataCmnData.headGearInventory;
                    break;
                case CLOTHES_GEAR:
                    p_curGearInventory = &saveDataCmnData.clothesInventory;
                    break;
                case SHOES_GEAR:
                    p_curGearInventory = &saveDataCmnData.shoeInventory;
                    break;
            }

            for (auto idx = 0u; idx < p_curGearInventory->size(); idx++) {
                auto& curGear = (*p_curGearInventory)[idx].gear;
                if (curGear.mGearId != curEquippedGearIds[gearKind]) {
                    continue;
                }

                m_equippedSkillOffsets[gearKind] = (size_t)&curGear.mMainSkillId - (size_t)&saveDataCmnData;

                m_displayGearsSkills[gearKind] = {
                    gear::GEAR_SKILL_NAME_MAP.at(curGear.mMainSkillId),
                    gear::GEAR_SKILL_NAME_MAP.at(curGear.mSubSkillIds[0]),
                    gear::GEAR_SKILL_NAME_MAP.at(curGear.mSubSkillIds[1]),
                    gear::GEAR_SKILL_NAME_MAP.at(curGear.mSubSkillIds[2]),
                    lx::ui::lv_btnmatrix::LV_BTNMATRIX_END_STR,
                };

                lv_btnmatrix_set_map(m_displayGearButtons[gearKind], m_displayGearsSkills[gearKind].data());
                break;
            }

            m_lastEquippedGearIds[gearKind] = curEquippedGearIds[gearKind];
        }
    } else {
        // set gear status to unavailable
        for (auto gearKind : DISPLAY_GEAR_KINDS) {
            m_displayGearsSkills[gearKind] = {GEAR_SKILL_UNAVAILABLE_STR, GEAR_SKILL_UNAVAILABLE_STR,
                                              GEAR_SKILL_UNAVAILABLE_STR, GEAR_SKILL_UNAVAILABLE_STR,
                                              lx::ui::lv_btnmatrix::LV_BTNMATRIX_END_STR};
            lv_btnmatrix_set_map(m_displayGearButtons[gearKind], m_displayGearsSkills[gearKind].data());
        }
    }

    m_basicScreen.processReturn();
}

void GearScreen::handleGearSkillButtonClick_(lv_obj_t* p_btnMatrix, lv_event_t event) {
    if (event != LV_EVENT_CLICKED) return;
    if (not MemoryReader::gameIsRunning()) return;

    auto curDisplayGearKindIdx = 0;
    for (auto p_curBtnMatrix : s_instance.m_displayGearButtons) {
        if (p_curBtnMatrix == p_btnMatrix) break;
        curDisplayGearKindIdx++;
    }

    lx::ui::Controller::show(SkillScreen::getInstance(
        s_instance.m_lastEquippedGearIds[curDisplayGearKindIdx], IN_GAME_GEAR_KIND_MAP[curDisplayGearKindIdx],
        reinterpret_cast<uint64_t>(save::getSaveDataCmnDataAddress()) +
            s_instance.m_equippedSkillOffsets[curDisplayGearKindIdx] +
            lv_btnmatrix_get_active_btn(p_btnMatrix) * sizeof(Cmn::Def::Gear::mMainSkillId),
        1));

    s_instance.m_lastEquippedGearIds[curDisplayGearKindIdx] = -1;  // invalidate current gear for reloading skills
}
