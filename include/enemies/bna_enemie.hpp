#pragma once

#include "bna_car.hpp"
#include "bn_random.h"

namespace bna {
    class Player;
    class Enemie {
        public:
        Enemie(bn::fixed_point pos);
        ~Enemie() = default;

        void update();
        void spawn(bn::vector<bna::Enemie, 4>& carros, bna::Player& player, bn::camera_ptr& camera, bn::size size);
        void checkCollision(bna::Car& car);

        bna::Car& getCar();

        private:
        bna::Car _cuerpo;

        bn::vector<bna::Enemie, 4>* _carros;
        bna::Player* _player;

        bn::random _random;
    };
} // namespace bna
