#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"

#include "bn_regular_bg_ptr.h"

namespace bna {
    class TitleScreen : public scene {
        public:
        TitleScreen();
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        TextManager _textStart;
        bn::regular_bg_ptr _background;
    };
}
