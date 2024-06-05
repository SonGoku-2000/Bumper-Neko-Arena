#include "bna_player.hpp"

#include "bn_keypad.h"
#include "bn_log.h"
namespace bna {
    constexpr bn::fixed VELOCIDAD = 1;
    constexpr bn::fixed ACELERACION = 0.1;
    constexpr bn::fixed FRICCION = 0.99;
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
        _rotation = _rotation.modulo(360);
    }
    else if (bn::keypad::right_held()) {
        _rotation -= GIRO;
        _rotation = _rotation.modulo(360);
    }

    if (bn::keypad::up_held()) {
        _eje = bna::Vector2(-1, 0);
    }
    else if (bn::keypad::down_held()) {
        _eje = bna::Vector2(1, 0);
    }
    else {
        _eje = bna::Vector2(0, 0);
    }


    bn::fixed_point newPos = _pos;
    _speed = _speed * FRICCION;
    _speed += ACELERACION * _eje.x();

    bna::Vector2 movimiento(0, _speed);
    movimiento = movimiento.rotate(_rotation);
    _dx = movimiento.x();

    _dy = movimiento.y();

    newPos.set_x(newPos.x() + _dx);
    newPos.set_y(newPos.y() + _dy);

    _hitbox.setRotation(_rotation);
    _hitbox.setPosition(newPos);

    bool bucleCompletado = true;
    for (int i = 0; i < _obstaculos->size(); i++) {
        if (_hitbox.checkCollision(_obstaculos->at(i))) {
            bucleCompletado = false;
        }
    }

    if (bucleCompletado) {
        _pos = newPos;
    }
    else {
        _hitbox.setPosition(_pos);
    }
}

void bna::Player::spawn(bn::vector<bna::Hitbox, 4>& obstaculos) {
    _obstaculos = &obstaculos;
}

