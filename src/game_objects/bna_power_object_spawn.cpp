#include "bna_power_object_spawn.hpp"

#include "bna_time.hpp"

#include "bna_car_powers_id.hpp"


bna::PowerObjectSpawn::PowerObjectSpawn(bn::fixed_point position, bn::camera_ptr& camera) :
    _camera(camera) {
    _position = bna::Indicator(position, true);
    _position.set_camera(camera);
    _remainingTime = _random.get_int(bna::time::seconds_to_frames(5), bna::time::seconds_to_frames(10));
}

void bna::PowerObjectSpawn::update() {
    if (_powerObject.has_value()) {
        return;
    }

    if (_remainingTime) {
        _remainingTime--;
        return;
    }

    _remainingTime = _random.get_int(bna::time::seconds_to_frames(5), bna::time::seconds_to_frames(10));
    _powerObject = bna::PowerObject(_position, _generateCarPowerId(), _camera);
}


bna::car_powers_id bna::PowerObjectSpawn::takePower() {
    bna::car_powers_id power = _powerObject->getCarPowerId();
    _powerObject.reset();
    return power;
}


bna::car_powers_id bna::PowerObjectSpawn::_generateCarPowerId() {
    return bna::car_powers_id(_random.get_int(1, int(bna::car_powers_id::MAX_VALUE)));
}

bool bna::PowerObjectSpawn::checkColission(bna::Hitbox& otherHitbox) {
    if (!_powerObject.has_value()) {
        return false;
    }
    return otherHitbox.checkCollision(_powerObject->get_hitbox());
}
