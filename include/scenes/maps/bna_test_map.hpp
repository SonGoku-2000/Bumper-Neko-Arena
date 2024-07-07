#pragma once

#include "bn_optional.h"
#include "bn_vector.h"

#include "bn_regular_bg_ptr.h"
#include "bn_camera_ptr.h"

#include "bn_size.h"
#include "bna_hitbox.hpp"
#include "bna_indicator.hpp"

#include "bna_enemie.hpp"
#include "bna_enemies_manager.hpp"
#include "bna_player.hpp"
#include "bna_scene.hpp"

#include "bna_position_icon_manager.hpp"

#include "bna_test_values.hpp"


namespace bna {
    class CarBuilder;
    enum class Characters :int;
    class Car;
    class TestMap : public scene {

        public:
        TestMap(CarBuilder& playerCarBuilder, Characters& playerCharacter);
        ~TestMap() = default;

        [[nodiscard]] bn::optional<scene_type> update() final;

        bn::size getSize();

        bn::vector<bna::Hitbox, 4>& getWalls();


        private:
        bn::size _size;
        bn::regular_bg_ptr _fondo;

        bn::vector<bna::Hitbox, 4> _walls;

        bn::vector<bna::Enemie, limit_values::MAX_ENEMIES> _enemies;
        bn::vector<bna::Car, limit_values::MAX_ENEMIES + 1> _cars;
        bn::vector<bna::Indicator, limit_values::MAX_ENEMIES + 1> _spawnPoints;
        bn::array<bn::fixed_point, limit_values::MAX_ENEMIES + 1> _ejes;

        bna::EnemiesManager _enemiesManager;
        bna::PositionIconManager _positionIconManager;

        bn::camera_ptr _camera;

        bna::Player _player;

        void _setCamera(bn::camera_ptr& camera);
        void _generateSpawnPoints();
        void _generateWalls();
        void _generatePlayer(CarBuilder& playerCarBuilder, Characters& playerCharacter);
        void _generateEnemies();
    };
} // namespace bna

