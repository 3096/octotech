#pragma once

#include "../game/gear.hpp"
#include "lx/debug.hpp"
#include "lx/ui/basic_screen_provider.hpp"

class SkillScreen : public lx::ui::IScreen {
    LOGCONSTRUCTM;

   private:
    SkillScreen();
    SkillScreen(const SkillScreen&) = delete;
    ~SkillScreen();
    static SkillScreen s_instance;

    lx::ui::BasicScreenProvider m_basicScreen;

    //
    decltype(Cmn::Def::Gear::mGearId) m_curGearId;
    Cmn::Def::GearKind m_curGearKind;
    uint64_t m_gearWriteAddress;

    // ui
    static constexpr auto BUTTONS_PER_LINE = 4;
    lv_obj_t* mp_buttonMatrix;
    std::vector<const char*> m_buttonMatrixStrs;
    std::vector<decltype(Cmn::Def::Gear::mMainSkillId)> m_buttonSkillIds;
    gear::EditingSlot m_editingSlot;

    static void handleButtonClick_(lv_obj_t* p_btnMatrix, lv_event_t event);

    // implement IScreen
    virtual inline void onMount(lx::ui::IScreen* prevScreen) override;

    virtual inline void renderScreen() override { return m_basicScreen.renderScreen(); }
    virtual inline void procFrame() override { return m_basicScreen.processReturn(); }

    virtual inline lv_obj_t* getLvScreenObj() override { return m_basicScreen.getLvScreenObj(); }
    virtual inline lv_group_t* getLvInputGroup() override { return m_basicScreen.getLvInputGroup(); }

   public:
    static inline SkillScreen& getInstance(decltype(Cmn::Def::Gear::mGearId) gearId, Cmn::Def::GearKind gearKind,
                                           uint64_t gearWriteAddress, gear::EditingSlot editingSlot) {
        s_instance.m_curGearId = gearId;
        s_instance.m_curGearKind = gearKind;
        s_instance.m_gearWriteAddress = gearWriteAddress;
        s_instance.m_editingSlot = editingSlot;
        return s_instance;
    }
};
