#include "theme.hpp"

#include "lx/debug.hpp"
#include "lx/ui/lv_helper.hpp"

Theme::Theme() {
    LOGSL("constructing... ");

    lv_style_init(&m_primaryColorBgStyle);
    lv_style_init(&m_btnMatrixBgStyle);
    lv_style_init(&m_btnMatrixBtnStyle);

    setPrimaryColorIpml_(DEFAULT_PRIMARY_COLOR);
    setSecondaryColorIpml_(DEFAULT_SECONDARY_COLOR);

    lv_style_set_border_width(&m_btnMatrixBtnStyle, LV_STATE_FOCUSED, 3);

    LOGEL("done");
}

Theme::~Theme() {}

void Theme::setPrimaryColorIpml_(lv_color_t primaryColor) {
    lv_style_set_bg_color(&m_primaryColorBgStyle, LV_STATE_DEFAULT, DEFAULT_PRIMARY_COLOR);

    lv_style_set_bg_color(&m_btnMatrixBtnStyle, LV_STATE_PRESSED, DEFAULT_PRIMARY_COLOR);

    lv_style_set_border_color(&m_btnMatrixBtnStyle, LV_STATE_FOCUSED, DEFAULT_PRIMARY_COLOR);
    lv_style_set_border_color(&m_btnMatrixBtnStyle, LV_STATE_FOCUSED | LV_STATE_PRESSED, DEFAULT_PRIMARY_COLOR);
}

void Theme::setSecondaryColorIpml_(lv_color_t secondaryColor) {
    lv_style_set_bg_color(&m_btnMatrixBgStyle, LV_STATE_DEFAULT, DEFAULT_SECONDARY_COLOR);
    lv_style_set_border_color(&m_btnMatrixBgStyle, LV_STATE_DEFAULT, DEFAULT_SECONDARY_COLOR);
    lv_style_set_border_color(&m_btnMatrixBgStyle, LV_STATE_FOCUSED, DEFAULT_SECONDARY_COLOR);

    lv_style_set_bg_color(&m_btnMatrixBtnStyle, LV_STATE_DEFAULT, DEFAULT_SECONDARY_COLOR);
    lv_style_set_border_color(&m_btnMatrixBtnStyle, LV_STATE_DEFAULT, DEFAULT_SECONDARY_COLOR);
}
