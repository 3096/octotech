#include "skill_screen.hpp"

#include "../game/memory.hpp"
#include "../theme.hpp"
#include "lx/ui/lv_helper.hpp"

SkillScreen SkillScreen::s_instance;

SkillScreen::SkillScreen() : LOGCONSTRUCT m_basicScreen(*this) {
    lv_obj_t* p_window = lx::ui::lv_win::create(getLvScreenObj(), Theme::getThemeColorBgStyle());
    m_basicScreen.addLvObjPositionUpdater(p_window, lx::ui::lv_win::updateFitParent);
    lv_win_set_title(p_window, "  Select Ability");

    mp_buttonMatrix =
        lx::ui::lv_btnmatrix::create(p_window, Theme::getBtnMatrixBgStyle(), Theme::getBtnMatrixBtnStyle());
    m_basicScreen.addLvObjPositionUpdater(mp_buttonMatrix, [p_window](lv_obj_t* p_btnMatrix) {
        lv_obj_set_size(p_btnMatrix, lx::ui::size::getScreenInnerWidth(), lx::ui::lv_win::getWinInnerHeight());
        lv_obj_align(p_btnMatrix, p_window, LV_ALIGN_CENTER, 0, lx::ui::lv_win::HEADER_HEIGHT() / 2);
    });
    lv_obj_set_event_cb(mp_buttonMatrix, handleButtonClick_);

    lv_group_add_obj(m_basicScreen.getLvInputGroup(), mp_buttonMatrix);

    LOG("done");
}

SkillScreen::~SkillScreen() {}

void SkillScreen::onMount(lx::ui::IScreen* prevScreen) {
    m_basicScreen.onMount(prevScreen);

    m_buttonMatrixStrs.clear();
    m_buttonSkillIds.clear();
    auto curCol = 0;
    for (auto skillId : gear::getGearSkills(m_curGearId, m_curGearKind, m_editingSlot)) {
        if (curCol == BUTTONS_PER_LINE) {
            m_buttonMatrixStrs.push_back(lx::ui::lv_btnmatrix::LV_BTNMATRIX_NEW_ROW_STR);
            curCol = 0;
        }

        m_buttonSkillIds.push_back(skillId);
        m_buttonMatrixStrs.push_back(gear::GEAR_SKILL_NAME_MAP.at(skillId));

        curCol++;
    }
    m_buttonMatrixStrs.push_back(lx::ui::lv_btnmatrix::LV_BTNMATRIX_END_STR);

    // set an empty map to force refresh
    lv_btnmatrix_set_map(mp_buttonMatrix, (const char**)&lx::ui::lv_btnmatrix::LV_BTNMATRIX_END_STR);

    lv_btnmatrix_set_map(mp_buttonMatrix, m_buttonMatrixStrs.data());
}

void SkillScreen::handleButtonClick_(lv_obj_t* p_btnMatrix, lv_event_t event) {
    if (event != LV_EVENT_CLICKED) return;

    auto skillToWrite = s_instance.m_buttonSkillIds[lv_btnmatrix_get_active_btn(p_btnMatrix)];

    auto addressToWrite = s_instance.m_gearWriteAddress + offsetof(Cmn::Def::Gear, mMainSkillId);
    auto writingSlotCount = 0u;
    auto totalSlotCount = decltype(Cmn::Def::Gear::mUnlockedSlotCount){};
    switch (s_instance.m_editingSlot) {
        case gear::EditingSlot::ALL:
            writingSlotCount = 4;
            totalSlotCount = 4;
            break;

        case gear::EditingSlot::ALL_SUB:
            addressToWrite += sizeof(decltype(skillToWrite));
            writingSlotCount = 3;
            totalSlotCount = 4;
            break;

        default:
            addressToWrite += sizeof(decltype(skillToWrite)) * static_cast<int>(s_instance.m_editingSlot);
            writingSlotCount = 1;
            totalSlotCount = static_cast<int>(s_instance.m_editingSlot) + 1;
    }

    // check and write slot counts
    auto curGearData = MemoryReader::readDataFromAddr<Cmn::Def::Gear>(
        reinterpret_cast<Cmn::Def::Gear*>(s_instance.m_gearWriteAddress));
    auto& curGear = *reinterpret_cast<Cmn::Def::Gear*>(&curGearData);

    if (curGear.mUnlockedSlotCount < totalSlotCount) {
        MemoryReader::writeToAddr<decltype(curGear.mUnlockedSlotCount)>(
            s_instance.m_gearWriteAddress + offsetof(Cmn::Def::Gear, mUnlockedSlotCount), totalSlotCount);
    }

    if (curGear.mUnlockedSlotCount < totalSlotCount) {
        MemoryReader::writeToAddr<decltype(curGear.mUnlockedSlotCount)>(
            s_instance.m_gearWriteAddress + offsetof(Cmn::Def::Gear, mUnlockedSlotCount), totalSlotCount);
    }

    // write the skills
    for (auto i = 0u; i < writingSlotCount; i++) {
        MemoryReader::writeToAddr<decltype(skillToWrite)>(addressToWrite + sizeof(decltype(skillToWrite)) * i,
                                                          skillToWrite);
    }

    s_instance.m_basicScreen.returnToPreviousScreen();
}
