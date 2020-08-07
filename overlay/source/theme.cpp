#include "theme.hpp"

#include "lx/debug.hpp"
#include "lx/ui/lv_helper.hpp"

Theme::Theme() {
    LOGSL("constructing... ");

    lx::ui::lv::initBgColorStyle(m_themeColorBgStyle, lv_color_hex(THEME_COLOR));

    LOGEL("done");
}

Theme::~Theme() {}
