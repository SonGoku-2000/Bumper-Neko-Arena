#pragma once

#include "bna_scene.hpp"
#include "bn_optional.h"

#include "bna_text_manager.hpp"
#include "bna_indicator.hpp"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_animate_actions.h"
#include "bn_sprite_animate_actions.h"

namespace bna {
    class MainMenu : public scene {
        public:
        MainMenu();
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        bn::regular_bg_ptr _playButton;
        bn::sprite_ptr _optionsButton;
        bn::sprite_ptr _backButton;
        bn::optional<bn::regular_bg_animate_action<10>> _animationPlayButton;
        bn::optional<bn::sprite_animate_action<10>> _animationOptionBackButton;

        bn::regular_bg_ptr _clouds;
        bn::regular_bg_ptr _background;
        bn::regular_bg_animate_action<10> _animation;

        enum class options :int {
            PLAY, OPTIONS, BACK
        };
        options _idOpcion;

        bn::vector<bna::Indicator, 5> _indicadores;

        void _updateSelectedOptionIcon();
        void _animatePressedButton();
    };
}
