#include "bna_camera_manager.hpp"

#include "bn_display.h"

#ifdef DEBUG
#include "bn_log.h"
#endif

bna::CameraManager::CameraManager(bn::camera_ptr& camera, bn::size size) :
    _camera(camera) {
    int ancho = size.width()/2;
    int alto = size.height()/2;
    setLimitLeft(-ancho);
    setLimitRight(ancho);
    setLimitUp(-alto);
    setLimitDown(alto);
}

void bna::CameraManager::update(bn::fixed_point pos) {
    if (pos.x() < _limitLeft) {
        pos.set_x(_limitLeft);
    }
    if (pos.x() > _limitRight) {
        pos.set_x(_limitRight);
    }

    if (pos.y() < _limitUp) {
        pos.set_y(_limitUp);
    }

    if (pos.y() > _limitDown) {
        pos.set_y(_limitDown);
    }

    _camera.set_position(pos);
}

void bna::CameraManager::setLimitLeft(int limiteIzquierda) {
    _limitLeft = limiteIzquierda + (bn::display::width() / 2);
}
void bna::CameraManager::setLimitRight(int limiteDerecha) {
    _limitRight = limiteDerecha - (bn::display::width() / 2);
}
void bna::CameraManager::setLimitUp(int limiteArriba) {
    _limitUp = limiteArriba + (bn::display::height() / 2);
}
void bna::CameraManager::setLimitDown(int limiteAbajo) {
    _limitDown = limiteAbajo - (bn::display::height() / 2);
}
