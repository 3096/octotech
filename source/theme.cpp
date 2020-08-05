#include "theme.hpp"

#include "lx/debug.hpp"
#include "lx/ui/lv_helper.hpp"

constexpr auto THEME_COLOR = 0x1976D2;

Theme::Theme() {
    LOGSL("constructing... ");

    lx::ui::lv::initBgColorStyle(m_themeColorBgStyle, lv_color_hex(THEME_COLOR));

    LOGEL("done");
}

Theme::~Theme() {}
