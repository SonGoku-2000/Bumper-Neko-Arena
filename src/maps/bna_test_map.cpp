#include "bna_test_map.hpp"

#include "bn_regular_bg_items_mapa_prueba.h"

bna::TestMap::TestMap() :
    _fondo(bn::regular_bg_items::mapa_prueba.create_bg(0, 0)) {
    _size = _fondo.dimensions();
// int separacion = 60;
    constexpr bool debug = true;
    // _walls.push_back(bna::Hitbox(bna::Vector2(0, 50), bna::Vector2(10,150), debug, 0));
    // _walls[0].setRotation(20);
    constexpr int separacion = 10;
    _walls.push_back(bna::Hitbox(bna::Vector2(0, (_size.height() / -2) + separacion), bna::Vector2(10, _size.width() - 10), debug, 0));
    _walls.push_back(bna::Hitbox(bna::Vector2(0, (_size.height() / 2) - separacion), bna::Vector2(10, _size.width() - 10), debug, 1));
    _walls.push_back(bna::Hitbox(bna::Vector2((_size.width() / 2) - separacion, 0), bna::Vector2(_size.height() - 10, 10), debug, 2));
    _walls.push_back(bna::Hitbox(bna::Vector2((_size.width() / -2) + separacion, 0), bna::Vector2(_size.height() - 10, 10), debug, 3));
}

void bna::TestMap::setCamera(bn::camera_ptr& camera) {
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
