#include "bna_test_map.hpp"

#include "bn_regular_bg_items_mapa_prueba.h"

#include "bna_scene_type.hpp"

#include "bn_core.h"


bna::TestMap::TestMap() :
    _fondo(bn::regular_bg_items::mapa_prueba.create_bg(0, 0)),
    _enemiesManager(_carros),
    _camera(bn::camera_ptr::create(0, 0)) {
    _size = _fondo.dimensions();


    constexpr bool debug = true;
    constexpr int separacion = 10;
    _walls.push_back(bna::Hitbox(bna::Vector2(0, (_size.height() / -2) + separacion), bna::Vector2(10, _size.width() - 10), debug, 0));
    _walls.push_back(bna::Hitbox(bna::Vector2(0, (_size.height() / 2) - separacion), bna::Vector2(10, _size.width() - 10), debug, 1));
    _walls.push_back(bna::Hitbox(bna::Vector2((_size.width() / 2) - separacion, 0), bna::Vector2(_size.height() - 10, 10), debug, 2));
    _walls.push_back(bna::Hitbox(bna::Vector2((_size.width() / -2) + separacion, 0), bna::Vector2(_size.height() - 10, 10), debug, 3));


    _carros.push_back(bna::Enemie(bn::fixed_point(60, 0)));
    _carros.push_back(bna::Enemie(bn::fixed_point(90, 0)));
    _carros.push_back(bna::Enemie(bn::fixed_point(120, 0)));

    _setCamera(_camera);

    _player.spawn(_carros, getWalls(), _camera, getSize());

    _enemiesManager.spawn(_carros, _player, getWalls(), _camera, getSize());
}


bn::optional<bna::scene_type> bna::TestMap::update() {
    while (true) {
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
