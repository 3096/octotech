#pragma once

#include "lvgl.h"

class Theme {
   private:
    Theme();
    Theme(const Theme&) = delete;
    ~Theme();
    inline static auto& getInstance() {
        static Theme s_instance;
        return s_instance;
    }

    lv_style_t m_themeColorBgStyle;

   public:
    static inline auto& getThemeColorBgStyle() { return getInstance().m_themeColorBgStyle; }
};
