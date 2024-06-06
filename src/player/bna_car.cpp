#include "bna_car.hpp"

namespace bna {
    constexpr bn::fixed VELOCIDAD = 1;
    constexpr bn::fixed ACELERACION = 0.1;
    constexpr bn::fixed FRICCION = 0.99;
    constexpr bn::fixed GIRO = 1.5;
} // namespace bna

bna::Car::Car(Hitbox hitbox, bn::fixed_point pos, bn::fixed peso) :
    _pos(pos),
    _peso(peso),
    _hitbox(hitbox) {
}

void bna::Car::update(bna::Vector2 eje) {
    _eje = eje;

    _rotation += eje.x() * GIRO;
    _rotation = _rotation.modulo(360);

    bn::fixed_point newPos = _pos;
    _speed = _speed * FRICCION;
    _speed += ACELERACION * _eje.y();

    bna::Vector2 movimiento(0, _speed);
    movimiento = movimiento.rotate(_rotation);
    _dx = movimiento.x();

    _dy = movimiento.y();

    newPos.set_x(newPos.x() + _dx);
    newPos.set_y(newPos.y() + _dy);

    _hitbox.setRotation(_rotation);
    _hitbox.setPosition(newPos);

    // bool bucleCompletado = true;
    // for (int i = 0; i < _obstaculos->size(); i++) {
    //     if (_hitbox.checkCollision(_obstaculos->at(i))) {
    //         bucleCompletado = false;
    //         _speed = -_speed;
    //     }
    // }

    // if (bucleCompletado) {
    _pos = newPos;
    // }
    // else {
    //     _hitbox.setPosition(_pos);
    // }
}