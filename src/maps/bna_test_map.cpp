#include "bna_test_map.hpp"

#include "bn_regular_bg_items_mapa_prueba.h"

bna::TestMap::TestMap() :
    _fondo(bn::regular_bg_items::mapa_prueba.create_bg(0, 0)) {
    _size = _fondo.dimensions();
}

void bna::TestMap::setCamera(bn::camera_ptr& camera) {
    _fondo.set_camera(camera);
}

bn::size bna::TestMap::getSize() {
    return _size;
}