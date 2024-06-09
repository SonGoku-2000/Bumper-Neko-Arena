#pragma once

#include "bn_camera_ptr.h"

#include "bna_hitbox.hpp"
#include "bna_vector2.hpp"

#include "bn_vector.h"
#include "bn_optional.h"

#include "bna_car.hpp"


namespace bna {
    class Player {
        public:
        Player();
        ~Player() = default;

        void update();
        void spawn(bn::vector<bna::Car, 4>& carros, bn::camera_ptr& camera);

        private:
        bna::Vector2 _eje;

        bn::vector<bna::Car, 4>* _carros;

        bna::Car _cuerpo;

        bn::optional<bn::camera_ptr> _camera;
    };
} // namespace bna

