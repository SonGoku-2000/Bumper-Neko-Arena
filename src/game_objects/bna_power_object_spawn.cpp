#include "bna_power_object_spawn.hpp"

#include "bna_time.hpp"

#include "bna_car_powers_id.hpp"

#include "bn_sprite_items_spawner_powers_open.h"
#include "bn_sprite_items_spawner_powers_close.h"

#include "bna_planes.hpp"

bn::random bna::PowerObjectSpawn::_random;

bna::PowerObjectSpawn::PowerObjectSpawn(bn::fixed_point position, bn::camera_ptr& camera) :
    _camera(camera),
    _sprite(bn::sprite_items::spawner_powers_open.create_sprite(position)) {
    _sprite.set_camera(_camera);
    _sprite.set_bg_priority(Planes::BACKGROUND);
    _sprite.put_below();
    _position = bna::Indicator(position, false);
    _position.set_camera(camera);
    _remainingTime = _random.get_int(bna::time::seconds_to_frames(5), bna::time::seconds_to_frames(10));

    _state = state::EMPTY;
    _fase = fase::START;
}

void bna::PowerObjectSpawn::update() {
    if (state::EMPTY == _state) {
        if (fase::START == _fase) {
            _fase = fase::READY;
            _remainingTime = _random.get_unbiased_int(bna::time::seconds_to_frames(4), bna::time::seconds_to_frames(10));
        }
        if (_remainingTime) {
            _remainingTime--;
            return;
        }
        _state = state::OPPENING;
        _fase = fase::START;
    }
    else if (state::OPPENING == _state) {
        if (fase::START == _fase) {
            _fase = fase::READY;
            _animation = bn::create_sprite_animate_action_once(
                _sprite,
                5,
                bn::sprite_items::spawner_powers_open.tiles_item(),
                0, 1, 1, 1, 1, 1, 2, 3, 4, 4
            );
            _powerObject = bna::PowerObject(_position, _generateCarPowerId(), _camera);
            _powerObject->put_below();
        }
        _animation->update();
        if (_animation->done()) {
            _state = state::READY;
            _fase = fase::START;
        }
    }
    else if (state::CLOSSIONG == _state) {
        if (fase::START == _fase) {
            _fase = fase::READY;
            _sprite.set_item(bn::sprite_items::spawner_powers_close, 4);
            _animation = bn::create_sprite_animate_action_once(
                _sprite,
                10,
                bn::sprite_items::spawner_powers_close.tiles_item(),
                4, 3, 2, 1, 0, 0
            );
        }
        _animation->update();
        if (_animation->done()) {
            _state = state::EMPTY;
            _fase = fase::START;
        }
    }
}


bna::car_powers_id bna::PowerObjectSpawn::takePower() {
    bna::car_powers_id power = _powerObject->getCarPowerId();
    _powerObject.reset();
    _state = state::CLOSSIONG;
    _fase = fase::START;
    return power;
}


bna::car_powers_id bna::PowerObjectSpawn::_generateCarPowerId() {
    return bna::car_powers_id(_random.get_int(1, int(bna::car_powers_id::MAX_VALUE)));
}

bool bna::PowerObjectSpawn::checkColission(bna::Hitbox& otherHitbox) {
    if (state::READY != _state) {
        return false;
    }
    return otherHitbox.checkCollision(_powerObject->get_hitbox());
}
