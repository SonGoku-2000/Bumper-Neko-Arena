#include "bna_camera_manager.hpp"

#include "bn_display.h"

#ifdef DEBUG
#include "bn_log.h"
#endif

#include "bna_vector2.hpp"

#include "bna_math.hpp"

bna::CameraManager::CameraManager(bn::camera_ptr& camera, bn::size size) :
    _camera(camera) {
    int ancho = size.width() / 2;
    int alto = size.height() / 2;
    setLimitLeft(-ancho);
    setLimitRight(ancho);
    setLimitUp(-alto);
    setLimitDown(alto);
    _previousPosition = _camera.position();
}

void bna::CameraManager::update(bn::fixed_point pos) {
    bn::fixed_point diferencia = pos - _previousPosition;
    _previousPosition = pos;


    bn::fixed aheadDistanceX = 90;
    bn::fixed aheadDistanceY = 55;
    bna::Vector2 aheadPosition;
    aheadPosition.set_x(pos.x() + diferencia.x() * aheadDistanceX);
    aheadPosition.set_y(pos.y() + diferencia.y() * aheadDistanceY);

    if (aheadPosition.x() < _limitLeft) {
        aheadPosition.set_x(_limitLeft);
    }
    if (aheadPosition.x() > _limitRight) {
        aheadPosition.set_x(_limitRight);
    }

    if (aheadPosition.y() < _limitUp) {
        aheadPosition.set_y(_limitUp);
    }

    if (aheadPosition.y() > _limitDown) {
        aheadPosition.set_y(_limitDown);
    }

    _camera.set_position(bna::math::lerp(_camera.position(), aheadPosition, bn::fixed(0.05)));
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
