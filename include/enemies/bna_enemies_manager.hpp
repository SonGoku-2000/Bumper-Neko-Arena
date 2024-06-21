#pragma once

#include "bn_vector.h"

#include "bna_enemie.hpp"
#include "bna_hitbox.hpp"

#include "bna_test_values.hpp"

namespace bna {
    class EnemiesManager {
        public:
        EnemiesManager(bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& enemies);
        ~EnemiesManager() = default;

        void update();
        void spawn(bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& carros, bna::Player& player, bn::vector<bna::Hitbox, 4>& _walls, bn::camera_ptr& camera, bn::size size);

        private:
        bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>* _enemies;
        bn::vector<bna::Hitbox, 4>* _walls;
    };
} // namespace bna
