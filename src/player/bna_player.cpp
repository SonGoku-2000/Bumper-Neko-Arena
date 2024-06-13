#include "bna_player.hpp"

#include "bn_keypad.h"
#include "bn_log.h"

#define DEBUG
#ifdef DEBUG
#define MOVIMIENTO_LIBRE
#ifdef MOVIMIENTO_LIBRE
constexpr bn::fixed VELOCIDAD_MOVIMIENTO_LIBRE = 2;
#endif
#endif


bna::Player::Player() :
    _cuerpo(bna::Hitbox(bna::Vector2(0, 0), bna::Vector2(10, 20), true), { 0,0 }, 5) {
}

void bna::Player::update() {
    _eje.set_x(int(bn::keypad::left_held()) - int(bn::keypad::right_held()));
    _eje.set_y(int(bn::keypad::down_held()) - int(bn::keypad::up_held()));

#ifdef MOVIMIENTO_LIBRE
    bn::fixed_point pos = _cuerpo.getPosition();
    if (bn::keypad::select_held()) {
        pos.set_x(pos.x() - (_eje.x() * VELOCIDAD_MOVIMIENTO_LIBRE));
        pos.set_y(pos.y() + (_eje.y() * VELOCIDAD_MOVIMIENTO_LIBRE));
        _cuerpo.setPosition(pos);
    }
    else {
        _cuerpo.update(_eje);
    }
#else
    _cuerpo.update(_eje);
#endif

    for (int i = 0; i < _enemies->size(); ++i) {
        _enemies->at(i).checkCollision(_cuerpo);
    }

    for (int i = 0; i < _walls->size(); i++) {
        _cuerpo.checkCollision(_walls->at(i));
    }

    _cameraManager->update(_cuerpo.getPosition());
}

void bna::Player::spawn(bn::vector<bna::Enemie, 4>& enemie, bn::vector<bna::Hitbox, 4>& walls, bn::camera_ptr& camera, bn::size size) {
    _enemies = &enemie;
    _walls = &walls;
    _cameraManager = bna::CameraManager(camera, size);
    _cuerpo.spawn(camera, size);
}

bn::fixed_point bna::Player::getPosition() {
    return _cuerpo.getPosition();
}

