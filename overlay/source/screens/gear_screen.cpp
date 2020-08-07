#include "gear_screen.hpp"

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
    for (auto gearKind : DISPLAY_GEAR_KINDS) {
        m_displayGearsSkills[gearKind] = {"0", "0", "0", "0", ""};  // temp:
        lv_btnmatrix_set_map(m_displayGearButtons[gearKind], m_displayGearsSkills[gearKind].data());
    }

    m_basicScreen.processReturn();
}
