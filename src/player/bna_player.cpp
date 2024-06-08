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
    // bool bucleCompletado = true;
    // for (int i = 0; i < _obstaculos->size(); i++) {
    //     if (_hitbox.checkCollision(_obstaculos->at(i))) {
    //         bucleCompletado = false;
    //         _speed = -_speed;
    //     }
    // }

    // if (bucleCompletado) {
    //     _pos = newPos;
    // }
    // else {
    //     _hitbox.setPosition(_pos);
    // }

    for (int i = 0; i < _carros->size(); ++i) {
        _carros->at(i).update({ 0,0 });
        if (_cuerpo.isColliding(_carros->at(i))) {
            _cuerpo.resolveCollision(_carros->at(i));
        }
    }
}

void bna::Player::spawn(bn::vector<bna::Car, 4>& carros) {
    _carros = &carros;
}

