#pragma once

#include "bn_vector.h"

#include "bna_enemie.hpp"


namespace bna {
    class EnemiesManager {
        public:
        EnemiesManager(bn::vector<bna::Enemie, 4>& enemies);
        ~EnemiesManager() = default;

        void update();
        void spawn(bn::vector<bna::Enemie, 4>& carros, bna::Player& player, bn::camera_ptr& camera, bn::size size);

        private:
        bn::vector<bna::Enemie, 4>* _enemies;
    };
} // namespace bna
