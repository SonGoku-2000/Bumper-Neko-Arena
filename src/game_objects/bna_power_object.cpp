#include "bna_power_object.hpp"

#include "bn_sprite_items_poderes_icono_mapa.h"

#include "bna_car_powers_id.hpp"


bna::PowerObject::PowerObject(bn::fixed_point position, bna::car_powers_id carPower, bn::camera_ptr& camera) :
    _sprite(bn::sprite_items::poderes_icono_mapa.create_sprite(position, int(carPower) - 1)) {
    _sprite.set_camera(camera);
    _carPower = carPower;
}

bna::PowerObject::PowerObject(bn::fixed_point position, bna::car_powers_id carPower) :
    _sprite(bn::sprite_items::poderes_icono_mapa.create_sprite(position, int(carPower) - 1)) {
    _carPower = carPower;
}


void bna::PowerObject::set_camera(bn::camera_ptr& camera) {
    _sprite.set_camera(camera);
}

bna::car_powers_id bna::PowerObject::getCarPowerId() {
    return _carPower;
}
