#include "bna_car.hpp"
#include "bn_math.h"

namespace bna {
    constexpr bn::fixed VELOCIDAD_MAX = 1;
    constexpr bn::fixed ACELERACION = 0.1;
    constexpr bn::fixed FRICCION = 0.99;
    constexpr bn::fixed GIRO = 1.5;
} // namespace bna

bna::Car::Car(Hitbox hitbox, bn::fixed_point pos, bn::fixed peso) :
    _pos(pos),
    _peso(peso),
    _hitbox(hitbox) {
    _externalForce = bn::fixed_point(0, 0);
    _dx = 0;
    _dy = 0;
}

void bna::Car::update(bna::Vector2 eje) {
    _eje = eje;

    _rotation += eje.x() * GIRO;
    _rotation = _rotation.modulo(360);

    bn::fixed_point newPos = _pos;
    _speed = _speed + ACELERACION * _eje.y();
    _speed = bn::max(bn::min(_speed, bna::VELOCIDAD_MAX), -bna::VELOCIDAD_MAX);

    if (_eje.y() == 0) {
        _speed *= bna::FRICCION;
    }

    bna::Vector2 movimiento(0, _speed);
    movimiento = movimiento.rotate(_rotation);
    _dx = movimiento.x();
    _dx += _externalForce.x();

    _dy = movimiento.y();
    _dy += _externalForce.y();

    _externalForce *= FRICCION;

    newPos.set_x(newPos.x() + _dx);
    newPos.set_y(newPos.y() + _dy);

    _hitbox.setRotation(_rotation);
    _hitbox.setPosition(newPos);
    
    _pos = newPos;
}

bna::Hitbox bna::Car::getHitbox() const {
    return _hitbox;
}

bool bna::Car::isColliding(const Car& other) const {
    return other.getHitbox().checkCollision(getHitbox());
}

bna::Vector2 bna::Car::getSpeed() {
    return Vector2(_dx, _dy);
}

bn::fixed_point bna::Car::getPosition() {
    return _pos;
}

bn::fixed bna::Car::getMass() {
    return _peso;
}

void bna::Car::applyExternalForce(bn::fixed_point externalForce) {
    _externalForce += externalForce;
}

void bna::Car::resolveCollision(Car& other) {
    bn::fixed dx = _pos.x() - other.getPosition().x();
    bn::fixed dy = _pos.y() - other.getPosition().y();
    bn::fixed distance = bn::sqrt(dx * dx + dy * dy);

    if (distance == 0) return;

    // // Normal vector
    bn::fixed nx = dx / distance;
    bn::fixed ny = dy / distance;

    // // Relative velocity
    bna::Vector2 relativeVelocity = getSpeed() - other.getSpeed();
    bn::fixed dotProduct = relativeVelocity.dot(bna::Vector2(nx, ny));

    // // If the particles are moving apart, no need to resolve collision
    if (dotProduct > 0) return;

    // Impulse scalar
    bn::fixed impulse = 2 * dotProduct / (getMass() + other.getMass());  // mass is 1 for both

    // Apply Force
    applyExternalForce(bn::fixed_point(
        -impulse * other.getMass() * nx,
        -impulse * other.getMass() * ny
    ));
    other.applyExternalForce(bn::fixed_point(
        impulse * getMass() * nx,
        impulse * getMass() * ny
    ));
}
