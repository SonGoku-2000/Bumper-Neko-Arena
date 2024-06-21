#include "bna_indicator.hpp"
#include "bn_sprite_items_indicator.h"
#include "bna_planes.hpp"

bna::Indicator::Indicator(bn::fixed_point pos, bool visible, int color) :
    bn::fixed_point(pos) {
    if (visible) {
        _sprite = bn::sprite_items::indicator.create_sprite(*this, color);
        _sprite->set_bg_priority(bna::Planes::FIRST);
        _sprite->put_above();
    }
}

bna::Indicator::Indicator(bn::fixed x, bn::fixed y, bool visible, int color) :
    Indicator(bn::fixed_point(x, y), visible, color) {
}

void bna::Indicator::set_camera(bn::camera_ptr camara) {
    if (_sprite.has_value()) {
        _sprite->set_camera(camara);
    }
}
