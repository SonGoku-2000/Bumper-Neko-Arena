#pragma once

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"

#include "bn_vector.h"

#include "bna_test_values.hpp"
#include "bn_fixed_rect.h"

namespace bna {
    class Enemie;
    class PositionIconManager {
        public:
        PositionIconManager(bn::camera_ptr& camera, bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& enemies);
        ~PositionIconManager() = default;

        void generateIcons();

        void update();

        private:
        bn::camera_ptr& _camera;
        bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>* _enemies;
        bn::vector<bn::sprite_ptr, limit_values::MAX_ENEMIES> _icons;
        const bn::fixed_rect _limite;
    };
} // namespace bna
