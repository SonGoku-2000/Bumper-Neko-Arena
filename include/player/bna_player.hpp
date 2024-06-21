#pragma once

#include "bn_camera_ptr.h"
#include "bna_camera_manager.hpp"

#include "bna_hitbox.hpp"
#include "bna_vector2.hpp"
#include "bn_size.h"

#include "bn_vector.h"
#include "bn_optional.h"

#include "bna_car.hpp"
#include "bna_enemie.hpp"

#include "bna_test_values.hpp"

namespace bna {
    class CarBuilder;
    class Player {
        public:
        Player(CarBuilder& body);
        ~Player() = default;

        void update();
        void spawn(bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& enemie, bn::vector<bna::Hitbox, 4>& _walls, bn::camera_ptr& camera, bn::size size);

        bn::fixed_point getPosition();
        
        void setBody(Car body);

        private:
        bna::Vector2 _eje;

        bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>* _enemies;

        bna::Car _cuerpo;

        bn::optional<bna::CameraManager> _cameraManager;

        bn::vector<bna::Hitbox, 4>* _walls;
    };
} // namespace bna

