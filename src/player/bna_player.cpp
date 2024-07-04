#include "bna_player.hpp"

#include "bn_keypad.h"

#include "bna_car_builder.hpp"

#define DEBUG
#ifdef DEBUG
#include "bn_log.h"
#define MOVIMIENTO_LIBRE
#ifdef MOVIMIENTO_LIBRE
constexpr bn::fixed VELOCIDAD_MOVIMIENTO_LIBRE = 2.5;
#endif
#endif
#include "bn_sound_items.h"


bna::Player::Player() {
}

void bna::Player::full_update() {
    _eje.set_x(int(bn::keypad::left_held()) - int(bn::keypad::right_held()));
    _eje.set_y(int(bn::keypad::down_held()) - int(bn::keypad::up_held()));

#ifdef MOVIMIENTO_LIBRE
    bn::fixed_point pos = _cuerpo->getPosition();
    if (bn::keypad::select_held()) {
        pos.set_x(pos.x() - (_eje.x() * VELOCIDAD_MOVIMIENTO_LIBRE));
        pos.set_y(pos.y() + (_eje.y() * VELOCIDAD_MOVIMIENTO_LIBRE));
        _cuerpo->setPosition(pos);
    }
    else {
        _cuerpo->update(_eje);
    }
#else
    _cuerpo->update(_eje);
#endif

    for (int i = 0; i < _enemies->size(); ++i) {
        _enemies->at(i).checkCollision(*_cuerpo);
    }
    bn::sound_items::turbo2.play();
    for (int i = 0; i < _walls->size(); i++) {
        _cuerpo->checkCollision(_walls->at(i));
    }

    _cameraManager->update(_cuerpo->getPosition());
}

void bna::Player::update() {
    _cameraManager->update(_cuerpo->getPosition());
}

bna::Vector2 bna::Player::getEje() {
    _eje.set_x(int(bn::keypad::left_held()) - int(bn::keypad::right_held()));
    _eje.set_y(int(bn::keypad::down_held()) - int(bn::keypad::up_held()));
    return _eje;
}

bna::Car& bna::Player::getCarRef() {
    return *_cuerpo;
}

void bna::Player::spawn(bn::vector<bna::Car, limit_values::MAX_ENEMIES + 1>& cars, bn::vector<bna::Hitbox, 4>& walls, int id_propia, bn::camera_ptr& camera, bn::size size) {
    _idPropia = id_propia;
    _enemies = &cars;
    _walls = &walls;
    _cameraManager = bna::CameraManager(camera, size);
    _cuerpo->spawn(camera, size);
}

void bna::Player::setBody(Car& body) {
    _cuerpo = &body;
}


bn::fixed_point bna::Player::getPosition() {
    return _cuerpo->getPosition();
}

