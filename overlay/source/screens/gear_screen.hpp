#pragma once

#include "Cmn/Def/Gear.h"
#include "lx/debug.hpp"
#include "lx/ui/basic_screen_provider.hpp"
#include "lx/ui/lv_helper.hpp"

class GearScreen : public lx::ui::IScreen {
    LOGCONSTRUCTM;

   private:
    GearScreen();
    GearScreen(const GearScreen&) = delete;
    ~GearScreen();
    static GearScreen s_instance;

    lx::ui::BasicScreenProvider m_basicScreen;

    static constexpr auto GEAR_KIND_COUNT = 3;
    enum DisplayGearKind { HEAD_GEAR, CLOTHES_GEAR, SHOES_GEAR };
    static constexpr auto IN_GAME_GEAR_KIND_MAP = std::array<Cmn::Def::GearKind, GEAR_KIND_COUNT>{
        Cmn::Def::GearKind::cHead, Cmn::Def::GearKind::cClothes, Cmn::Def::GearKind::cShoes};

    static constexpr auto GEAR_SKILL_COUNT = 4;
    using EquippedGearIds = std::array<decltype(Cmn::Def::Gear::mGearId), GEAR_KIND_COUNT>;

    // game
    EquippedGearIds m_lastEquippedGearIds;
    std::array<size_t, GEAR_KIND_COUNT> m_equippedSkillOffsets;

    // ui
    static constexpr auto DISPLAY_GEAR_KINDS = {HEAD_GEAR, CLOTHES_GEAR, SHOES_GEAR};
    static constexpr auto GEAR_KIND_LABEL_STRS = std::array<const char*, GEAR_KIND_COUNT>{"Head", "Clothes", "Shoes"};

    static constexpr auto GEAR_SKILL_BUTTON_HEIGHT = 48;
    static constexpr auto GEAR_SKILL_BUTTON_SINK = 30;

    using GearSkillStrs = std::array<const char*, GEAR_SKILL_COUNT + 1>;
    static constexpr auto GEAR_SKILL_UNAVAILABLE_STR = "-";

    std::array<lv_obj_t*, GEAR_KIND_COUNT> m_displayGearLabels;
    std::array<lv_obj_t*, GEAR_KIND_COUNT> m_displayGearButtons;
    std::array<GearSkillStrs, GEAR_KIND_COUNT> m_displayGearsSkills;

    static inline auto getOneGearHeight_() {
        return (lx::ui::size::getScreenHeight() - lx::ui::lv_win::HEADER_HEIGHT() - lx::ui::size::MARGIN()) /
               GEAR_KIND_COUNT;
    }

    static inline auto getDisplayGearY_(DisplayGearKind displayGearKind) {
        return lx::ui::lv_win::HEADER_HEIGHT() + lx::ui::size::MARGIN() + getOneGearHeight_() * displayGearKind;
    }

    static void handleGearSkillButtonClick_(lv_obj_t* btnm, lv_event_t event);

    // implement IScreen
    virtual inline void onMount(lx::ui::IScreen* prevScreen) override { return m_basicScreen.onMount(nullptr); }

    virtual inline void renderScreen() override { return m_basicScreen.renderScreen(); }
    virtual inline void procFrame() override;

    virtual inline lv_obj_t* getLvScreenObj() override { return m_basicScreen.getLvScreenObj(); }
    virtual inline lv_group_t* getLvInputGroup() override { return m_basicScreen.getLvInputGroup(); }

    static const lx::LvKeyMap LV_KEY_MAP;
    virtual inline const lx::LvKeyMap& getLvKeyMap() override { return LV_KEY_MAP; }

   public:
    static inline GearScreen& getInstance() { return s_instance; }
};
