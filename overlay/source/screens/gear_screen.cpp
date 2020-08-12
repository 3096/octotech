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

        auto p_btnMatrix =
            lx::ui::lv_btnmatrix::create(p_window, Theme::getBtnMatrixBgStyle(), Theme::getBtnMatrixBtnStyle());
        m_basicScreen.addLvObjPositionUpdater(p_btnMatrix, [p_window, gearKind](lv_obj_t* p_btnMatrix) {
            lv_obj_set_size(p_btnMatrix, lx::ui::size::getScreenWidth() - lx::ui::size::MARGIN() * 2,
                            lx::ui::size::coord(GEAR_SKILL_BUTTON_HEIGHT));
            lv_obj_align(p_btnMatrix, p_window, LV_ALIGN_IN_TOP_MID, 0,
                         getDisplayGearY_(gearKind) + lx::ui::size::coord(GEAR_SKILL_BUTTON_SINK));
        });
        m_displayGearButtons[gearKind] = p_btnMatrix;
        lv_group_add_obj(m_basicScreen.getLvInputGroup(), p_btnMatrix);
        lv_obj_set_event_cb(p_btnMatrix, handleGearSkillButtonClick_);
        m_displayGearsButtonMaps[gearKind][4] = lx::ui::lv_btnmatrix::LV_BTNMATRIX_NEW_ROW_STR;
        m_displayGearsButtonMaps[gearKind][5] = GEAR_EDIT_ALL_SUB_STR;
        m_displayGearsButtonMaps[gearKind][6] = GEAR_EDIT_ALL_STR;
        m_displayGearsButtonMaps[gearKind][7] = lx::ui::lv_btnmatrix::LV_BTNMATRIX_END_STR;
    }

    LOG("done");
}

GearScreen::~GearScreen() {}

void GearScreen::procFrame() {
    m_basicScreen.processReturn();

    // process up/down key inputs to switch gear
    auto lvInputGroup = m_basicScreen.getLvInputGroup();
    if (lx::ui::Controller::keyComboIsJustPressed(KEY_DDOWN) and
        lv_btnmatrix_get_active_btn(lv_group_get_focused(lvInputGroup)) >= gear::GEAR_SKILL_COUNT) {
        lv_group_focus_next(lvInputGroup);

    } else if (lx::ui::Controller::keyComboIsJustPressed(KEY_DUP) and
               lv_btnmatrix_get_active_btn(lv_group_get_focused(lvInputGroup)) < gear::GEAR_SKILL_COUNT) {
        lv_group_focus_prev(lvInputGroup);
    }

    // update gear
    if (not MemoryReader::gameIsRunning()) {
        return setGearUnavailable_();
    }

    try {
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

                m_equippedGearOffsets[gearKind] = (size_t)&curGear - (size_t)&saveDataCmnData;

                m_displayGearsButtonMaps[gearKind][0] = gear::GEAR_SKILL_NAME_MAP.at(curGear.mMainSkillId);
                m_displayGearsButtonMaps[gearKind][1] = gear::GEAR_SKILL_NAME_MAP.at(curGear.mSubSkillIds[0]);
                m_displayGearsButtonMaps[gearKind][2] = gear::GEAR_SKILL_NAME_MAP.at(curGear.mSubSkillIds[1]);
                m_displayGearsButtonMaps[gearKind][3] = gear::GEAR_SKILL_NAME_MAP.at(curGear.mSubSkillIds[2]);

                lv_btnmatrix_set_map(m_displayGearButtons[gearKind], m_displayGearsButtonMaps[gearKind].data());
                break;
            }

            m_lastEquippedGearIds[gearKind] = curEquippedGearIds[gearKind];
        }
    } catch (std::runtime_error& e) {
        setGearUnavailable_();
    }
}

void GearScreen::setGearUnavailable_() {
    for (auto gearKind : DISPLAY_GEAR_KINDS) {
        m_displayGearsButtonMaps[gearKind][0] = GEAR_SKILL_UNAVAILABLE_STR;
        m_displayGearsButtonMaps[gearKind][1] = GEAR_SKILL_UNAVAILABLE_STR;
        m_displayGearsButtonMaps[gearKind][2] = GEAR_SKILL_UNAVAILABLE_STR;
        m_displayGearsButtonMaps[gearKind][3] = GEAR_SKILL_UNAVAILABLE_STR;
        lv_btnmatrix_set_map(m_displayGearButtons[gearKind], m_displayGearsButtonMaps[gearKind].data());
    }
    m_lastEquippedGearIds = {};
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
            s_instance.m_equippedGearOffsets[curDisplayGearKindIdx],
        static_cast<gear::EditingSlot>(lv_btnmatrix_get_active_btn(p_btnMatrix))));

    s_instance.m_lastEquippedGearIds[curDisplayGearKindIdx] = -1;  // invalidate current gear for reloading skills
}
