#include "bna_car.hpp"
#include "bn_math.h"

#include "bn_sprite_items_carro_base.h"
#include "bn_log.h"

namespace bna {
    constexpr bn::fixed VELOCIDAD_MAX = 3;
    constexpr bn::fixed ACELERACION = 0.1;
    constexpr bn::fixed FRICCION = 0.99;
    constexpr bn::fixed FRICCION_LADO = 0.2;
    constexpr bn::fixed MULTIPLICADOR_REBOTE = 1.5;
    constexpr bn::fixed GIRO = 1.5;
} // namespace bna

bna::Car::Car(Hitbox hitbox, bn::fixed_point pos, bn::fixed peso) :
    _pos(pos),
    _peso(peso),
    _hitbox(hitbox),
    _sprite(bn::sprite_items::carro_base.create_sprite(pos)) {
    _externalForce = bn::fixed_point(0, 0);
    _dx = 0;
    _dy = 0;
    _rotation = 0;
}

void bna::Car::spawn(bn::camera_ptr& camera, bn::size tamanoMapa) {
    _sprite.set_camera(camera);
    _hitbox.setCamera(camera);
    _mapBorders = tamanoMapa;
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
    // _dx += _externalForce.x();

    _dy = movimiento.y();
    // _dy += _externalForce.y();

    _externalForce *= FRICCION;

    newPos.set_x(newPos.x() + _dx);
    newPos.set_y(newPos.y() + _dy);

    newPos.set_x(newPos.x() + _externalForce.x());
    newPos.set_y(newPos.y() + _externalForce.y());

    _hitbox.setRotation(_rotation);
    _hitbox.setPosition(newPos);

    _pos = newPos;
    _checkBorders();

    _sprite.set_position(_pos);
    if (_rotation < 0) {
        _sprite.set_rotation_angle(_rotation + 360);
    }
    else {
        _sprite.set_rotation_angle(_rotation);
    }
}

bna::Hitbox bna::Car::getHitbox() const {
    return _hitbox;
}

void bna::Car::checkCollision(bna::Car& otherCar) {
    if (isColliding(otherCar)) {
        resolveCollision(otherCar);
    }
}

bool bna::Car::isColliding(const Car& other) const {
    return other.getHitbox().checkCollision(getHitbox());
}

bna::Vector2 bna::Car::getSpeed() {
    return Vector2(_dx+_externalForce.x(), _dy+_externalForce.y());
}

bn::fixed_point bna::Car::getPosition() {
    return _pos;
}

bn::fixed bna::Car::getMass() {
    return _peso;
}

void bna::Car::applyExternalForce(bn::fixed_point externalForce) {
    _externalForce += externalForce * MULTIPLICADOR_REBOTE;
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

void bna::Car::_checkBorders() {
    const bn::vector<bna::Vector2, 4> vertices = getHitbox().getVertices();
    int width = _mapBorders.width() / 2;
    int height = _mapBorders.height() / 2;

    if (_pos.x() < -width) {
        _pos.set_x(-width + bn::max(_hitbox.height(), _hitbox.width()));
    }
    else if (_pos.x() > width) {
        _pos.set_x(width - bn::max(_hitbox.height(), _hitbox.width()));
    }

    if (_pos.y() < -height) {
        _pos.set_y(-height + bn::max(_hitbox.height(), _hitbox.width()));
    }
    else if (_pos.y() > height) {
        _pos.set_y(height - bn::max(_hitbox.height(), _hitbox.width()));
    }

    _hitbox.setPosition(_pos);

    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].x() < -width || vertices[i].x() > width) {
            applyExternalForce(bn::fixed_point(-_dx, 0));
            return;
        }
        if (vertices[i].y() < -height || vertices[i].y() > height) {
            applyExternalForce(bn::fixed_point(0, -_dy));
            return;
        }
    }
}
