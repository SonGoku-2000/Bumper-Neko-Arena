#pragma once

#include "bna_hitbox.hpp"
#include "bn_vector.h"

namespace bna {
    class Player {
        public:
        Player();
        ~Player() = default;

        void update();
        void spawn(bn::vector<bna::Hitbox,4>& obstaculos);

        private:
        bn::fixed_point _pos;
        Hitbox _hitbox;
        bn::vector<bna::Hitbox,4>* _obstaculos;
    };
} // namespace bna

