#pragma once

#include "bna_car.hpp"
#include "bn_random.h"

#include "bna_test_values.hpp"

namespace bna {
    class Player;
    class CarBuilder;
    class Enemie {
        public:
        Enemie(CarBuilder& body);
        ~Enemie() = default;

        void update();
        void spawn(bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& carros, bna::Player& player, bn::vector<bna::Hitbox, 4>& walls, bn::camera_ptr& camera, bn::size size);
        void checkCollision(bna::Car& car);
        void checkCollision(bna::Enemie& car);
        void checkCollision(bna::Hitbox& hitbox);

        bna::Car& getCar();

        private:
        bna::Car _cuerpo;

        bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>* _carros;
        bna::Player* _player;
        bn::vector<bna::Hitbox, 4>* _walls;

        bn::random _random;
    };
} // namespace bna
