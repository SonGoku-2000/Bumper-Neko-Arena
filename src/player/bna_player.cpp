#include "bna_player.hpp"

#include "bn_keypad.h"
#include "bn_log.h"



bna::Player::Player() :
    _cuerpo(bna::Hitbox(bna::Vector2(0, 0), bna::Vector2(10, 20), true), { 0,0 }, 5) {
}

void bna::Player::update() {
    _eje.set_x(int(bn::keypad::left_held()) - int(bn::keypad::right_held()));
    _eje.set_y(int(bn::keypad::down_held()) - int(bn::keypad::up_held()));

    _cuerpo.update(_eje);
}

void bna::Player::spawn(bn::vector<bna::Hitbox, 4>& obstaculos) {
    _obstaculos = &obstaculos;
}

