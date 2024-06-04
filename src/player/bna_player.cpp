#include "bna_player.hpp"

#include "bn_keypad.h"

namespace bna {
    constexpr bn::fixed VELOCIDAD = 1;
    constexpr bn::fixed GIRO = 1.5;
} // namespace bna


bna::Player::Player() :
    _hitbox(bna::Vector2(0, 0), bna::Vector2(10, 20), true) {
    _rotation = 0;
    _eje = bna::Vector2(1, 0);
    _eje.rotate(_rotation);
}

void bna::Player::update() {
    if (bn::keypad::left_held()) {
        _rotation += GIRO;
    }
    else if (bn::keypad::right_held()) {
        _rotation -= GIRO;
    }

    if (bn::keypad::up_held()) {
        _eje = bna::Vector2(0, -1);
    }
    else if (bn::keypad::down_held()) {
        _eje = bna::Vector2(0, 1);
    }
    else {
        _eje = bna::Vector2(0, 0);
    }

    _eje = _eje.rotate(_rotation);
    _pos.set_x(_pos.x() + (_eje.x() * VELOCIDAD));
    _pos.set_y(_pos.y() + (_eje.y() * VELOCIDAD));

    _hitbox.setRotation(_rotation);
    _hitbox.setPosition(_pos);
}

void bna::Player::spawn(bn::vector<bna::Hitbox, 4>& obstaculos) {
    _obstaculos = &obstaculos;
}

