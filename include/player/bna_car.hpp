#pragma once

#include "bn_fixed_point.h"
#include "bn_fixed.h"

#include "bna_vector2.hpp"
#include "bna_hitbox.hpp"

namespace bna {
    class Car {
        public:
        Car(Hitbox hitbox, bn::fixed_point pos, bn::fixed peso);
        ~Car() = default;

        void update(bna::Vector2 eje);

        private:
        bn::fixed_point _pos;
        bn::fixed _rotation; // en grados
        bna::Vector2 _eje;

        bn::fixed _speed;
        bn::fixed _dx;
        bn::fixed _dy;

        bn::fixed _peso;

        Hitbox _hitbox;
    };
} // namespace bna

