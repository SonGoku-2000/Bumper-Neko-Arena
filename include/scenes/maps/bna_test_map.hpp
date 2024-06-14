#pragma once

#include "bn_optional.h"
#include "bn_vector.h"

#include "bn_regular_bg_ptr.h"
#include "bn_camera_ptr.h"

#include "bn_size.h"
#include "bna_hitbox.hpp"

#include "bna_enemie.hpp"
#include "bna_enemies_manager.hpp"
#include "bna_player.hpp"
#include "bna_scene.hpp"


namespace bna {
    class TestMap : public scene {

        public:
        TestMap();
        ~TestMap() = default;

        [[nodiscard]] bn::optional<scene_type> update() final;

        bn::size getSize();

        bn::vector<bna::Hitbox, 4>& getWalls();


        private:
        bn::size _size;
        bn::regular_bg_ptr _fondo;

        bn::vector<bna::Hitbox, 4> _walls;

        bn::vector<bna::Enemie, 4> _carros;

        bna::EnemiesManager _enemiesManager;

        bn::camera_ptr _camera;

        bna::Player _player;

        void _setCamera(bn::camera_ptr& camera);
    };
} // namespace bna

