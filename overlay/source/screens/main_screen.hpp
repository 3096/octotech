#pragma once

#include "lx/debug.hpp"
#include "lx/ui/basic_screen_provider.hpp"

class MainScreen : public lx::ui::IScreen {
    LOGCONSTRUCTM;

   private:
    MainScreen();
    MainScreen(const MainScreen&) = delete;
    ~MainScreen();
    static MainScreen s_instance;

    lx::ui::BasicScreenProvider m_basicScreen;

    // implement IScreen
    virtual inline void onMount(lx::ui::IScreen* prevScreen) override { return m_basicScreen.onMount(prevScreen); }

    virtual inline void renderScreen() override { return m_basicScreen.renderScreen(); }
    virtual inline void procFrame() override { return m_basicScreen.processReturn(); }

    virtual inline lv_obj_t* getLvScreenObj() override { return m_basicScreen.getLvScreenObj(); }
    virtual inline lv_group_t* getLvInputGroup() override { return m_basicScreen.getLvInputGroup(); }

   public:
    static inline MainScreen& getInstance() { return s_instance; }
};
