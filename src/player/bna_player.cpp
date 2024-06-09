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

    for (int i = 0; i < _carros->size(); ++i) {
        _carros->at(i).update({ 0,0 });
        _carros->at(i).checkCollision(_cuerpo);
    }
    _cameraManager->update(_cuerpo.getPosition());
}

void bna::Player::spawn(bn::vector<bna::Car, 4>& carros,bn::camera_ptr& camera) {
    _carros = &carros;
    _cameraManager = bna::CameraManager(camera,bn::size(300,300));
    _cuerpo.spawn(camera);
}

