#pragma once

#include "lvgl.h"

class Theme {
   private:
    static constexpr auto DEFAULT_PRIMARY_COLOR = lv_color_t{0x88, 0x47, 0xff, 0xff};  // #ff4788
    static constexpr auto DEFAULT_SECONDARY_COLOR = lv_color_t{0x41, 0xb0, 0, 0xef};   // #00b041

    Theme();
    Theme(const Theme&) = delete;
    ~Theme();
    inline static auto& getInstance() {
        static Theme s_instance;
        return s_instance;
    }

    lv_style_t m_primaryColorBgStyle;
    lv_style_t m_btnMatrixBgStyle;
    lv_style_t m_btnMatrixBtnStyle;

    void setPrimaryColorIpml_(lv_color_t primaryColor);
    void setSecondaryColorIpml_(lv_color_t secondaryColor);

   public:
    static inline void setPrimaryColor(lv_color_t primaryColor) { getInstance().setPrimaryColorIpml_(primaryColor); }
    static inline void setSecondaryColor(lv_color_t secondaryColor) {
        getInstance().setSecondaryColorIpml_(secondaryColor);
    }

    static inline auto& getThemeColorBgStyle() { return getInstance().m_primaryColorBgStyle; }
    static inline auto& getBtnMatrixBgStyle() { return getInstance().m_btnMatrixBgStyle; }
    static inline auto& getBtnMatrixBtnStyle() { return getInstance().m_btnMatrixBtnStyle; }
};
