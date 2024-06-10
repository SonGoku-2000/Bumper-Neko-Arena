#pragma once

#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"

#include "bn_fixed_point.h"
#include "bn_fixed.h"
#include "bn_size.h"

#include "bna_vector2.hpp"
#include "bna_hitbox.hpp"

namespace bna {
    class Car {
        public:
        Car(Hitbox hitbox, bn::fixed_point pos, bn::fixed peso);
        ~Car() = default;

        void update(bna::Vector2 eje);

        void spawn(bn::camera_ptr& camera, bn::size tamanoMapa);

        void checkCollision(Car& otherCar);

        bool isColliding(const Car& other) const;

        void resolveCollision(Car& other);

        bna::Hitbox getHitbox() const;

        bn::fixed_point getPosition();
        void setPosition(bn::fixed_point position);

        bn::fixed getRotation() const;

        bna::Vector2 getSpeed();

        bn::fixed getMass();

        void applyExternalForce(bn::fixed_point force);

        private:
        bn::fixed_point _pos;
        bn::fixed _rotation; // en grados
        bna::Vector2 _eje;

        bn::fixed _speed;
        bn::fixed _dx;
        bn::fixed _dy;
        bn::fixed_point _externalForce;

        bn::fixed _peso;

        Hitbox _hitbox;

        bn::sprite_ptr _sprite;

        bn::size _mapBorders;

        void _checkBorders();
    };
} // namespace bna

