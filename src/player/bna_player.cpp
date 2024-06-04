#include "bna_player.hpp"

#include "bn_keypad.h"

bna::Player::Player() :
    _hitbox(bna::Vector2(0, 0), bna::Vector2(10, 20), true) {
}

void bna::Player::update() {
    if (bn::keypad::left_held()) {
        _pos -= bn::fixed_point(1, 0);
    }
    else if (bn::keypad::right_held()) {
        _pos += bn::fixed_point(1, 0);
    }

    if (bn::keypad::up_held()) {
        _pos -= bn::fixed_point(0, 1);
    }
    else if (bn::keypad::down_held()) {
        _pos += bn::fixed_point(0, 1);
    }
    _hitbox.setPosition(_pos);
}

void bna::Player::spawn(bn::vector<bna::Hitbox, 4>& obstaculos) {
    _obstaculos = &obstaculos;
}

