#include "bna_car.hpp"
#include "bn_math.h"

#include "bn_sprite_items_carro_base.h"
#include "bn_log.h"
#include "bna_colissions.hpp"

#ifdef PROFILE
#include "bn_profiler.h"
#endif

namespace bna {
    namespace default_values {
        constexpr bn::fixed MAX_SPEED = 3;
        constexpr bn::fixed ACELERATION = 0.1;
        constexpr bn::fixed TURN = 1.5;
    } // namespace default
    constexpr bn::fixed FRICCION = 0.99;
    constexpr bn::fixed FRICCION_LADO = 0.2;
    constexpr bn::fixed MULTIPLICADOR_REBOTE = 1.5;
} // namespace bna

bna::Car::Car(Hitbox hitbox, bn::fixed_point pos, bn::fixed weight) :
    _pos(pos),
    _maxSpeed(bna::default_values::MAX_SPEED),
    _aceleration(bna::default_values::ACELERATION),
    _turn(bna::default_values::TURN),
    _weight(weight),
    _hitbox(hitbox),
    _sprite(bn::sprite_items::carro_base.create_sprite(pos)) {
    _externalForce = bn::fixed_point(0, 0);
    _dx = 0;
    _dy = 0;
    _rotation = 0;
}

bna::Car::Car(Hitbox hitbox, bn::fixed_point pos, bn::fixed maxSpeed, bn::fixed aceleration, bn::fixed turn, bn::fixed weight) :
    _pos(pos),
    _maxSpeed(maxSpeed),
    _aceleration(aceleration),
    _turn(turn),
    _weight(weight),
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
#ifdef PROFILE
    BN_PROFILER_START("Car update");
#endif
    _eje = eje;

    _rotation += eje.x() * _turn;
    _rotation = _rotation.modulo(360);

    bn::fixed_point newPos = _pos;
    _speed = _speed + _aceleration * _eje.y();
    _speed = bn::max(-_maxSpeed, bn::min(_speed, _maxSpeed));

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
    _sprite.set_rotation_angle(getRotation());

#ifdef PROFILE
    BN_PROFILER_STOP();
#endif
}


void bna::Car::checkCollision(bna::Car& otherCar) {
    if (isColliding(otherCar)) {
        resolveCollision(otherCar);
    }
}

void bna::Car::checkCollision(bna::Hitbox& otherHitbox) {
    if (otherHitbox.checkCollision(getHitbox())) {
        bna::CollisionPoint collisionPoint = isColliding(otherHitbox);
        resolveCollision(collisionPoint);
    }
}

bool bna::Car::isColliding(Car& other) {
    return other.getHitbox().checkCollision(getHitbox());
}

bna::CollisionPoint bna::Car::isColliding(bna::Hitbox& other) {
    return other.checkCollisionPoint(getHitbox());
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
        // bn::vector<bna::Vector2, 4> vertices2 = hb2.getVertices();
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

void bna::Car::resolveCollision(bna::CollisionPoint collisionPoint) {
    bn::fixed dx = collisionPoint.correctionVector.x();
    bn::fixed dy = collisionPoint.correctionVector.y();
    bn::fixed distance = bn::sqrt(dx * dx + dy * dy);

    if (distance == 0) return;

    // // Normal vector
    bn::fixed nx = dx / distance;
    bn::fixed ny = dy / distance;

    // // Relative velocity
    bna::Vector2 relativeVelocity = getSpeed();
    bn::fixed dotProduct = relativeVelocity.dot(bna::Vector2(nx, ny));

    // // If the particles are moving apart, no need to resolve collision
    if (dotProduct > 0) return;

    // Impulse scalar
    bn::fixed impulse = 2 * dotProduct / (getMass() + getMass());  // mass is 1 for both

    // Apply Force
    applyExternalForce(bn::fixed_point(
        -impulse * getMass() * nx,
        -impulse * getMass() * ny
    ));
    _pos += collisionPoint.correctionVector;
}


void bna::Car::applyExternalForce(bn::fixed_point externalForce) {
    _externalForce += externalForce * MULTIPLICADOR_REBOTE;
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


bna::Hitbox& bna::Car::getHitbox() {
    return _hitbox;
}

bna::Vector2 bna::Car::getSpeed() {
    return Vector2(_dx + _externalForce.x(), _dy + _externalForce.y());
}

bn::fixed_point bna::Car::getPosition() {
    return _pos;
}

void bna::Car::setPosition(bn::fixed_point position) {
    _pos = position;
    _hitbox.setPosition(position);
    _sprite.set_position(position);
}

bn::fixed bna::Car::getMass() {
    return _weight;
}

bn::fixed bna::Car::getRotation() const {
    if (_rotation < 0) {
        return _rotation + 360;
    }
    else {
        return _rotation;
    }
}



