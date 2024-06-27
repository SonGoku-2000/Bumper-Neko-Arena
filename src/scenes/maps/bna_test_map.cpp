#include "bna_test_map.hpp"

#include "bn_regular_bg_items_mapa_prueba.h"

#include "bna_scene_type.hpp"

#include "bn_core.h"
#include "bna_car_builder.hpp"
#include "bna_parts.hpp"

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

bna::TestMap::TestMap(CarBuilder& player) :
    _fondo(bn::regular_bg_items::mapa_prueba.create_bg(0, 0)),
    _enemiesManager(_carros),
    _camera(bn::camera_ptr::create(0, 0)),
    _player(player) {
    _size = _fondo.dimensions();


    constexpr bool debug = true;
    constexpr int separacion = 10;
    _walls.push_back(bna::Hitbox(bna::Vector2(0, (_size.height() / -2) + separacion), bna::Vector2(10, _size.width() - 10), debug, 0));
    _walls.push_back(bna::Hitbox(bna::Vector2(0, (_size.height() / 2) - separacion), bna::Vector2(10, _size.width() - 10), debug, 1));
    _walls.push_back(bna::Hitbox(bna::Vector2((_size.width() / 2) - separacion, 0), bna::Vector2(_size.height() - 10, 10), debug, 2));
    _walls.push_back(bna::Hitbox(bna::Vector2((_size.width() / -2) + separacion, 0), bna::Vector2(_size.height() - 10, 10), debug, 3));
    // _walls.push_back(bna::Hitbox(bna::Vector2(60, 0), bna::Vector2(70, 10), debug, 3));

    CarBuilder car_builder;
    car_builder.body = bna::parts::bodys::MEDIUM;
    car_builder.motor = bna::parts::motors::SLOW;
    car_builder.wheel = bna::parts::wheels::NORMAL;

    car_builder.position = bn::fixed_point(60, 0);
    _carros.push_back(car_builder);

    car_builder.position = bn::fixed_point(90, 0);
    _carros.push_back(car_builder);

    car_builder.position = bn::fixed_point(120, 0);
    _carros.push_back(car_builder);

    _setCamera(_camera);

    _player.spawn(_carros, getWalls(), _camera, getSize());

    _enemiesManager.spawn(_carros, _player, getWalls(), _camera, getSize());
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
        _player.update();
            _enemiesManager.update();
            bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

void bna::TestMap::_setCamera(bn::camera_ptr& camera) {
    _fondo.set_camera(camera);
    for (int i = 0; i < _walls.size(); i++) {
        _walls[i].setCamera(camera);
    }
}

bn::size bna::TestMap::getSize() {
    return _size;
}

bn::vector<bna::Hitbox, 4>& bna::TestMap::getWalls() {
    return _walls;
}
