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

#include "bna_test_values.hpp"
namespace bna {
    class CarBuilder;
    class Car;
    class TestMapLink : public scene {

        public:
        TestMapLink(CarBuilder& player,int id_propia);
        ~TestMapLink() = default;

        [[nodiscard]] bn::optional<scene_type> update() final;

        bn::size getSize();

        bn::vector<bna::Hitbox, 4>& getWalls();


        private:
        bn::size _size;
        bn::regular_bg_ptr _fondo;

        bn::vector<bna::Hitbox, 4> _walls;

        bn::vector<bna::Car, limit_values::MAX_ENEMIES + 1> _cars;
        bn::array<bn::fixed_point, limit_values::MAX_ENEMIES + 1> _ejes;

        bn::camera_ptr _camera;

        bna::Player _player;
        const int _idPropia;

        void _setCamera(bn::camera_ptr& camera);
    };
} // namespace bna

