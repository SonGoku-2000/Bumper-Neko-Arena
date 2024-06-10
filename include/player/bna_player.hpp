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


namespace bna {
    class Player {
        public:
        Player();
        ~Player() = default;

        void update();
        void spawn(bn::vector<bna::Enemie, 4>& enemie, bn::camera_ptr& camera, bn::size size);

        private:
        bna::Vector2 _eje;

        bn::vector<bna::Enemie, 4>* _enemies;

        bna::Car _cuerpo;

        bn::optional<bna::CameraManager> _cameraManager;

    };
} // namespace bna

