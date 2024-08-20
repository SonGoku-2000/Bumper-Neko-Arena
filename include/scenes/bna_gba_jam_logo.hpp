#pragma once

#include "bna_scene.hpp"

#include "bn_regular_bg_ptr.h"

namespace bna {
    class GbaJamLogo : public scene {
        public:
        GbaJamLogo();
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        bn::regular_bg_ptr _background;
        enum estados { APARECIENDO, ESPERANDO_VISIBLE, DESEPARECIENDO, ESPERANDO_OCULTO };
        estados _estado;
    };
}
