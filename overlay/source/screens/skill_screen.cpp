#include "skill_screen.hpp"

#include "../game/memory.hpp"
#include "../theme.hpp"
#include "lx/ui/lv_helper.hpp"

SkillScreen SkillScreen::s_instance;

SkillScreen::SkillScreen() : LOGCONSTRUCT m_basicScreen(*this) {
    lv_obj_t* p_window = lx::ui::lv_win::create(getLvScreenObj(), Theme::getThemeColorBgStyle());
    m_basicScreen.addLvObjPositionUpdater(p_window, lx::ui::lv_win::updateFitParent);
    lv_win_set_title(p_window, "  Select Ability");

    mp_buttonMatrix = lx::ui::lv_btnmatrix::create(p_window);
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
    for (auto& skillIdEntry : gear::GEAR_SKILL_NAME_MAP) {
        if (not gear::skillIdIsValid(skillIdEntry.first, m_curGearId, m_curGearKind)) {
            continue;
        }

        if (curCol == BUTTONS_PER_LINE) {
            m_buttonMatrixStrs.push_back(lx::ui::lv_btnmatrix::LV_BTNMATRIX_NEW_ROW_STR);
            m_buttonSkillIds.push_back(-2);
            curCol = 0;
        }

        m_buttonMatrixStrs.push_back(skillIdEntry.second);
        m_buttonSkillIds.push_back(skillIdEntry.first);

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
    for (auto i = 0u; i < s_instance.m_writingSlotCount; i++) {
        MemoryReader::writeToAddr<decltype(skillToWrite)>(
            s_instance.m_skillWriteAddress + sizeof(decltype(skillToWrite)) * i, skillToWrite);
    }

    s_instance.m_basicScreen.returnToPreviousScreen();
}
