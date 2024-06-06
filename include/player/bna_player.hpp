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
        void spawn(bn::vector<bna::Car, 4>& carros);

        private:
        bna::Vector2 _eje;

        bn::vector<bna::Car, 4>* _carros;

        bna::Car _cuerpo;
    };
} // namespace bna

