#pragma once

#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"

#include "bn_fixed_point.h"
#include "bn_fixed.h"
#include "bn_size.h"

#include "bna_vector2.hpp"
#include "bna_hitbox.hpp"

namespace bna {
    struct CollisionPoint;
    struct Stats;
    enum class car_powers_id :int;
    class Car {
        public:
        Car(Hitbox hitbox, bn::fixed_point position, bn::fixed weight);
        Car(Hitbox hitbox, bn::fixed_point position, bn::fixed maxSpeed, bn::fixed aceleration, bn::fixed turn, bn::fixed weight);
        Car(Hitbox hitbox, bn::fixed_point position, Stats stats);
        ~Car() = default;

        void update(bna::Vector2 eje);

        void spawn(bn::camera_ptr& camera, bn::size tamanoMapa);

        void checkCollision(Car& otherCar);
        void checkCollision(Hitbox& otherCar);

        bool isColliding(Car& other);
        CollisionPoint isColliding(Hitbox& other);

        void resolveCollision(Car& other);
        void resolveCollision(CollisionPoint collisionPoint);

        bna::Hitbox& getHitbox();

        bn::fixed_point getPosition();
        void setPosition(bn::fixed_point position);
        void setPositionX(bn::fixed rotaion);
        void setPositionY(bn::fixed rotaion);

        bn::fixed getRotation() const;
        void setRotation(bn::fixed rotaion);

        bna::Vector2 getVelocity();
        bn::fixed getSpeed();
        bn::fixed_point getExternalForce();
        void setSpeed(bn::fixed speed);
        void setExternalForce(bn::fixed_point external_force);

        bn::fixed getMass();

        void applyExternalForce(bn::fixed_point force);

        void applyDamage(bn::fixed damage);

        bool isAlive();
        bn::fixed getLife();

        bool isCrash();

        void usePower(car_powers_id car_power);

        bool hasSpikes();

        private:
        bn::fixed_point _pos;
        bn::fixed _rotation; // en grados
        bna::Vector2 _eje;

        bn::fixed _speed;
        bn::fixed _dx;
        bn::fixed _dy;
        bn::fixed_point _externalForce;

        bn::fixed _maxSpeed;
        bn::fixed _aceleration;
        bn::fixed _turn;
        bn::fixed _weight;

        bn::fixed _life;

        Hitbox _hitbox;

        bn::sprite_ptr _sprite;

        bn::size _mapBorders;

        enum state {
            LIFE,
            DEATH
        };

        state _state;

        bool _crash;

        car_powers_id _active_power;
        int _elapsedTimeActivePower;

        void _checkBorders();
        void _hurt(Car& other);
        void _checkTimePower();
    };
} // namespace bna

