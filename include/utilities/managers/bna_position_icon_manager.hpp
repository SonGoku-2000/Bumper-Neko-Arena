#pragma once

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"

#include "bn_vector.h"

#include "bna_test_values.hpp"
#include "bn_fixed_rect.h"

namespace bna {
    class Enemie;
    class Vector2;
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
        bn::vector<bn::sprite_ptr, limit_values::MAX_ENEMIES> _iconsBack;
        const bn::fixed_rect _limite;

        bn::fixed_point _getIconPosition(bn::fixed_point car_objetive);
        int _getIconFrame(bna::Vector2 car_objetive);
    };
} // namespace bna
