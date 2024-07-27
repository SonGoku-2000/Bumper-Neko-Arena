#include "bna_power_object.hpp"

#include "bn_sprite_items_poderes_icono_mapa.h"

#include "bna_car_powers_id.hpp"


namespace bna {
    constexpr bool SHOW_HITBOX = true;
} // namespace bna



bna::PowerObject::PowerObject(bn::fixed_point position, bna::car_powers_id carPower, bn::camera_ptr& camera) :
    _sprite(bn::sprite_items::poderes_icono_mapa.create_sprite(position, int(carPower) - 1)),
    _hitbox(bna::Hitbox(position, bna::Vector2(16, 16), SHOW_HITBOX, 0)) {
    set_camera(camera);
    _carPower = carPower;
}

bna::PowerObject::PowerObject(bn::fixed_point position, bna::car_powers_id carPower) :
    _sprite(bn::sprite_items::poderes_icono_mapa.create_sprite(position, int(carPower) - 1)),
    _hitbox(bna::Hitbox(position, bna::Vector2(16, 16), SHOW_HITBOX, 0)) {
    _carPower = carPower;
}


void bna::PowerObject::set_camera(bn::camera_ptr& camera) {
    _sprite.set_camera(camera);
    _hitbox.setCamera(camera);
}

bna::car_powers_id bna::PowerObject::getCarPowerId() {
    return _carPower;
}

bna::Hitbox& bna::PowerObject::get_hitbox() {
    return _hitbox;
}
