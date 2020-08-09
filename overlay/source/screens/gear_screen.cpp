#include "gear_screen.hpp"

#include "../game/gear.hpp"
#include "../game/save.hpp"
#include "../theme.hpp"

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
            lv_obj_set_size(p_btnMatrix, lx::ui::size::getLayerWidth() - lx::ui::size::MARGIN() * 2,
                            lx::ui::size::coord(GEAR_SKILL_BUTTON_HEIGHT));
            lv_obj_align(p_btnMatrix, p_window, LV_ALIGN_IN_TOP_MID, 0,
                         getDisplayGearY_(gearKind) + lx::ui::size::coord(GEAR_SKILL_BUTTON_SINK));
        });
        m_displayGearButtons[gearKind] = p_btnMatrix;
    }

    LOG("done");
}

GearScreen::~GearScreen() {}

void GearScreen::procFrame() {
    if (MemoryReader::gameIsRunning()) {
        auto saveDataCmnDataBuffer = save::getSaveDataCmnData();
        auto& saveDataCmnData = *reinterpret_cast<Cmn::SaveDataCmn::SaveDataCmnData*>(saveDataCmnDataBuffer.get());

        auto curEquippedGearIds = EquippedGearIds{saveDataCmnData.equippedHeadGearId,
                                                  saveDataCmnData.equippedClothesId, saveDataCmnData.equippedShoesId};

        for (auto gearKind : DISPLAY_GEAR_KINDS) {
            if (m_lastEquippedGearIds[gearKind] != curEquippedGearIds[gearKind]) {
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
                    auto curGear = (*p_curGearInventory)[idx].gear;
                    if (curGear.mGearId != curEquippedGearIds[gearKind]) {
                        continue;
                    }

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
            }
        }
    } else {
        for (auto gearKind : DISPLAY_GEAR_KINDS) {
            m_displayGearsSkills[gearKind] = {GEAR_SKILL_UNAVAILABLE_STR, GEAR_SKILL_UNAVAILABLE_STR,
                                              GEAR_SKILL_UNAVAILABLE_STR, GEAR_SKILL_UNAVAILABLE_STR,
                                              lx::ui::lv_btnmatrix::LV_BTNMATRIX_END_STR};  // temp:
            lv_btnmatrix_set_map(m_displayGearButtons[gearKind], m_displayGearsSkills[gearKind].data());
        }
    }

    m_basicScreen.processReturn();
}
