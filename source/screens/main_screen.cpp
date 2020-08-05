#include "main_screen.hpp"

#include "../theme.hpp"
#include "lx/ui/lv_helper.hpp"

MainScreen MainScreen::s_instance;

MainScreen::MainScreen() : LOGCONSTRUCT m_basicScreen(*this) {
    lv_obj_t* p_window = lx::ui::lv_win::create(getLvScreenObj(), Theme::getThemeColorBgStyle());
    m_basicScreen.addLvObjPositionUpdater(p_window, lx::ui::lv_win::updateFitParent);
    lv_win_set_title(p_window, "  luxio overlay");

    lv_obj_t* p_label = lx::ui::lv_label::create(p_window);
    m_basicScreen.addLvObjPositionUpdater(p_label, [](lv_obj_t* mp_promptLabel) {
        lv_obj_align(mp_promptLabel, nullptr, LV_ALIGN_IN_TOP_LEFT, lx::ui::size::MARGIN(), lx::ui::size::MARGIN());
    });
    lv_label_set_text(p_label, "Hello world!");

    LOG("done");
}

MainScreen::~MainScreen() {}
