#pragma once

#include "bna_hitbox.hpp"
#include "bna_vector2.hpp"
#include "bn_vector.h"

#include "bna_car.hpp"


namespace bna {
    class Player {
        public:
        Player();
        ~Player() = default;

        void update();
        void spawn(bn::vector<bna::Hitbox, 4>& obstaculos);

        private:
        bna::Vector2 _eje;

        bn::vector<bna::Hitbox, 4>* _obstaculos;

        bna::Car _cuerpo;
    };
} // namespace bna

