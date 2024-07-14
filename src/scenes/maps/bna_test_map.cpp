#include "bna_test_map.hpp"

#include "bn_regular_bg_items_mapa_prueba.h"

#include "bna_scene_type.hpp"

#include "bn_core.h"
#include "bn_random.h"
#include "bna_characters.hpp"
#include "bna_car_builder.hpp"
#include "bna_parts.hpp"
#include "bna_car.hpp"

#define MOVE_ENEMIES

#define DEBUG_CPU
#ifdef DEBUG_CPU
constexpr int CPU_CICLES = 64;
#include "bn_log.h"
#ifdef BN_CFG_PROFILER_ENABLED
#include "bn_profiler.h"
#include "bn_keypad.h"
// #define PROFILE
#endif
#endif

#include "bn_music_items.h"
#include "bn_sound_items.h"

bna::TestMap::TestMap(CarBuilder& playerCarBuilder, Characters& playerCharacter) :
    _fondo(bn::regular_bg_items::mapa_prueba.create_bg(0, 0)),
    _enemiesManager(_enemies),
    _positionIconManager(_camera, _enemies),
    _camera(bn::camera_ptr::create(0, 0)) {
    _size = _fondo.dimensions();


    _generateWalls();

    _generateSpawnPoints();

    _generatePlayer(playerCarBuilder, playerCharacter);

    _setCamera(_camera);

    _generateEnemies();

    _positionIconManager.generateIcons();
}

void bna::TestMap::_generateSpawnPoints() {
    constexpr bool debug = true;
    _spawnPoints.push_back(bna::Indicator(200, 200, debug));
    _spawnPoints.push_back(bna::Indicator(-200, 200, debug));
    _spawnPoints.push_back(bna::Indicator(-200, -200, debug));
    _spawnPoints.push_back(bna::Indicator(200, -200, debug));
    _spawnPoints.push_back(bna::Indicator(0, 0, debug));

    for (int i = 0; i < _spawnPoints.size(); i++) {
        _spawnPoints[i].set_camera(_camera);
    }
}

void bna::TestMap::_generateWalls() {
    constexpr bool debug = true;
    constexpr int separacion = 10;
    _walls.push_back(bna::Hitbox(bna::Vector2(0, (_size.height() / -2) + separacion), bna::Vector2(10, _size.width() - 10), debug, 0));
    _walls.push_back(bna::Hitbox(bna::Vector2(0, (_size.height() / 2) - separacion), bna::Vector2(10, _size.width() - 10), debug, 1));
    _walls.push_back(bna::Hitbox(bna::Vector2((_size.width() / 2) - separacion, 0), bna::Vector2(_size.height() - 10, 10), debug, 2));
    _walls.push_back(bna::Hitbox(bna::Vector2((_size.width() / -2) + separacion, 0), bna::Vector2(_size.height() - 10, 10), debug, 3));
    // _walls.push_back(bna::Hitbox(bna::Vector2(60, 0), bna::Vector2(70, 10), debug, 3));

    for (int i = 0; i < _walls.size(); i++) {
        _walls[i].setCamera(_camera);
    }
}

void bna::TestMap::_generatePlayer(CarBuilder& playerCarBuilder, Characters& playerCharacter) {
    playerCarBuilder.position = _spawnPoints[0];
    _cars.push_back(playerCarBuilder.build());
    _player.setBody(_cars[0]);
    _player.setCharacter(playerCharacter);

    _player.spawn(_cars, getWalls(), 0, _camera, getSize());
    _uiLife.setCar(_cars[0]);
}

void bna::TestMap::_generateEnemies() {
    CarBuilder car_builder;
    car_builder.body = bna::parts::bodys::MEDIUM;
    car_builder.motor = bna::parts::motors::SLOW;
    car_builder.wheel = bna::parts::wheels::NORMAL;

    car_builder.position = _spawnPoints[_cars.size()];
    _cars.push_back(car_builder.build());
    _enemies.push_back(_cars.back());

    car_builder.position = _spawnPoints[_cars.size()];
    _cars.push_back(car_builder.build());
    _enemies.push_back(_cars.back());

    car_builder.position = _spawnPoints[_cars.size()];
    _cars.push_back(car_builder.build());
    _enemies.push_back(_cars.back());

    for (int i = 0; i < _enemies.size(); i++) {
        _enemies[i].setCharacter(bna::Characters(i));
    }

    for (int i = 0; i < _cars.size(); i++) {
        _cars[i].spawn(_camera, getSize());
    }

    _enemiesManager.spawn(_cars, getWalls(), _camera, getSize());
}



bn::optional<bna::scene_type> bna::TestMap::update() {
#ifdef DEBUG_CPU
    int cpuCont = 0;
    bn::fixed cpu = 0;
#endif
    bn::music_items::forward.play();
    while (true) {
#ifdef DEBUG_CPU
        if (cpuCont == CPU_CICLES) {
            BN_LOG("CPU : % ", cpu / CPU_CICLES * 100);
            cpu = 0;
            cpuCont = 0;
        }
        else {
            cpu += bn::core::last_cpu_usage();
            cpuCont++;
        }
#endif
#ifdef PROFILE
        if (bn::keypad::l_held() and
            bn::keypad::r_held() and
            bn::keypad::start_held()) {
            bn::profiler::show();
        }
#endif

        _ejes[0] = _player.getEje();

#ifdef MOVE_ENEMIES
        for (int i = 0; i < _enemies.size(); i++) {
            _ejes[i + 1] = _enemies[i].getEje();
        }
#endif


        for (int id_car = 0; id_car < _cars.size(); id_car++) {
            _cars[id_car].update(_ejes[id_car]);
            for (int id_wall = 0; id_wall < _walls.size(); id_wall++) {
                _cars[id_car].checkCollision(_walls[id_wall]);
            }

            for (int id_other = id_car + 1; id_other < _cars.size(); id_other++) {
                _cars[id_car].checkCollision(_cars.at(id_other));
            }
            if (_cars[id_car].isCrash()) {
                bn::sound_items::crash.play();
            }
        }

        _player.update();
        _uiLife.update();
        _positionIconManager.update();
        // _enemiesManager.update();
        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

void bna::TestMap::_setCamera(bn::camera_ptr& camera) {
    _fondo.set_camera(camera);
}

bn::size bna::TestMap::getSize() {
    return _size;
}

bn::vector<bna::Hitbox, 4>& bna::TestMap::getWalls() {
    return _walls;
}
