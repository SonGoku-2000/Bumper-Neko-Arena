#include "bna_car.hpp"
#include "bn_math.h"

#include "bn_sprite_items_carro_base.h"
#include "bna_colissions.hpp"

#include "bna_parts.hpp"
#include "bna_car_powers_id.hpp"
#include "bna_test_values.hpp"

#include "bn_sprite_items_cat_black_driving.h"
#include "bn_sprite_items_cat_persian_drivin.h"
#include "bn_sprite_items_cat_siamese_drivin.h"
#include "bn_sprite_items_cat_tricolour_driving.h"

#include "bna_cats_id.hpp"



#define DEBUG
#ifdef  DEBUG
#include "bn_log.h"
#endif
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
    Car(hitbox, pos, bna::Stats(bna::default_values::MAX_SPEED, bna::default_values::ACELERATION, bna::default_values::TURN, weight)) {
}

bna::Car::Car(Hitbox hitbox, bn::fixed_point pos, bn::fixed maxSpeed, bn::fixed aceleration, bn::fixed turn, bn::fixed weight) :
    Car(hitbox, pos, bna::Stats(maxSpeed, aceleration, turn, weight)) {
}

bna::Car::Car(Hitbox hitbox, bn::fixed_point pos, Stats stats) :
    _hitbox(hitbox) {

    _catId = bna::cats_id::TRICOLOUR;
    _setSprite();
    _setAnimation();

    _maxSpeed = stats.maxSpeed;
    _aceleration = stats.aceleration;
    _turn = stats.turn;
    _weight = stats.weight;

    _pos = pos;
    _externalForce = bn::fixed_point(0, 0);
    _dx = 0;
    _dy = 0;
    _rotation = 0;

    _life = bna::limit_values::MAX_LIFE;
    _state = state::LIFE;

    _crash = false;

    _active_power = bna::car_powers_id::NONE;
    _elapsedTimeActivePower = 0;
}


void bna::Car::spawn(bn::camera_ptr& camera, bn::size tamanoMapa) {
    _sprite->set_camera(camera);
    _hitbox.setCamera(camera);
    _mapBorders = tamanoMapa;
}

void bna::Car::update(bna::Vector2 eje) {
#ifdef PROFILE
    BN_PROFILER_START("Car update");
#endif
    _crash = false;
    switch (_state) {
        case state::LIFE: {
            _eje = eje;

            _rotation += eje.x() * _turn;
            _rotation = _rotation.modulo(360);

            bn::fixed_point newPos = _pos;
            _speed = _speed + _aceleration * _eje.y();
            _speed = bn::max(-_maxSpeed, bn::min(_speed, _maxSpeed));

            if (_eje.y() == 0) {
                _speed *= bna::FRICCION;
            }

            if (_speed == 0) {
                _resetSprite();
            }
            else{
                _animation->update();
            }

            bna::Vector2 movimiento(0, _speed);
            if (bna::car_powers_id::TURBO == _active_power) {
                bn::fixed TURBO_POWER = 2;
                movimiento = movimiento * TURBO_POWER;
            }
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

            _sprite->set_position(_pos);
            _sprite->set_rotation_angle(getRotation());

            if (_life <= 0) {
                _sprite->set_visible(false);
                _state = state::DEATH;
            }

            _checkTimePower();
            break;
        }

        case state::DEATH:
            break;

        default:
            break;
    }


#ifdef PROFILE
    BN_PROFILER_STOP();
#endif
}



void bna::Car::_hurt(bna::Car& other) {
    bn::fixed speedA = getSpeed();
    bn::fixed speedB = other.getSpeed();

    Vector2 relativeVelocity = getVelocity() - other.getVelocity();
    bn::fixed relativeSpeed = relativeVelocity.length();

    bn::fixed damageToB = speedA * relativeSpeed;
    bn::fixed damageToA = speedB * relativeSpeed;

    if (other.hasSpikes()) {
        applyDamage(damageToA + damageToB);
    }
    else {
        applyDamage(damageToA);
    }

    if (hasSpikes()) {
        other.applyDamage(damageToB + damageToA);
    }
    else {
        other.applyDamage(damageToB);
    }
}

void bna::Car::applyDamage(bn::fixed damage) {
    if (bna::car_powers_id::ARMOR == _active_power) {
        constexpr bn::fixed ARMOR = 0.5;
        damage = damage * ARMOR;
    }
    // BN_LOG("Dano:", damage);
    _life -= bn::abs(damage);
}

bool bna::Car::hasSpikes() {
    return bna::car_powers_id::SPIKE == _active_power;
}

void bna::Car::checkCollision(bna::Car& otherCar) {
    if (!isAlive() or !otherCar.isAlive()) {
        return;
    }

    if (isColliding(otherCar)) {
        resolveCollision(otherCar);
        _hurt(otherCar);
        _crash = true;
    }
}

void bna::Car::checkCollision(bna::Hitbox& otherHitbox) {
    if (!isAlive()) {
        return;
    }

    if (otherHitbox.checkCollision(getHitbox())) {
        bna::CollisionPoint collisionPoint = isColliding(otherHitbox);
        resolveCollision(collisionPoint);
        _crash = true;
    }
}

bool bna::Car::isColliding(Car& other) {
    return other.getHitbox().checkCollision(getHitbox());
}

bna::CollisionPoint bna::Car::isColliding(bna::Hitbox& other) {
    return other.checkCollisionPoint(getHitbox());
}

bool bna::Car::isCollidingFast(bna::Hitbox& other) {
    return other.checkCollision(getHitbox());
}

bool bna::Car::isAlive() {
    return _life > 0;
}

bn::fixed bna::Car::getLife() {
    return _life;
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
    bna::Vector2 relativeVelocity = getVelocity() - other.getVelocity();
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
    bna::Vector2 relativeVelocity = getVelocity();
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

bna::Vector2 bna::Car::getVelocity() {
    return Vector2(_dx + _externalForce.x(), _dy + _externalForce.y());
}

bn::fixed bna::Car::getSpeed() {
    return _speed;
}

bn::fixed_point bna::Car::getExternalForce() {
    return _externalForce;
}

void bna::Car::setSpeed(bn::fixed speed) {
    _speed = speed;
}

void bna::Car::setExternalForce(bn::fixed_point external_force) {
    _externalForce = external_force;
}

bn::fixed_point bna::Car::getPosition() {
    return _pos;
}

void bna::Car::setPosition(bn::fixed_point position) {
    _pos = position;
    _hitbox.setPosition(position);
    _sprite->set_position(position);
}

void bna::Car::setPositionX(bn::fixed x) {
    setPosition(bn::fixed_point(x, getPosition().y()));
}

void bna::Car::setPositionY(bn::fixed y) {
    setPosition(bn::fixed_point(getPosition().x(), y));
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

void bna::Car::setRotation(bn::fixed rotation) {
    _rotation = rotation;
}

bool bna::Car::isCrash() {
    return _crash;
}

void bna::Car::usePower(bna::car_powers_id car_power) {
    _active_power = car_power;
    _elapsedTimeActivePower = 0;
}

void bna::Car::_checkTimePower() {
    if (_elapsedTimeActivePower == 180) {
        _elapsedTimeActivePower = 0;
        _active_power = bna::car_powers_id::NONE;
    }
    else {
        _elapsedTimeActivePower++;
    }
}


void bna::Car::_setSprite() {
    if (bna::cats_id::TRICOLOUR == _catId) {
        _sprite = bn::sprite_items::cat_tricolour_driving.create_sprite(_pos);
    }
}

void bna::Car::_setAnimation() {
    if (bna::cats_id::TRICOLOUR == _catId) {
        _animation = bn::create_sprite_animate_action_forever(_sprite.value(), 8, bn::sprite_items::cat_tricolour_driving.tiles_item(), 0, 1, 2, 1, 0, 3, 4, 3);
    }
}

void bna::Car::_resetSprite(){
    if(bna::cats_id::TRICOLOUR==_catId){
        _sprite->set_tiles(bn::sprite_items::cat_tricolour_driving.tiles_item());
    }
}

