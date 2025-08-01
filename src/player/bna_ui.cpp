#include "bna_ui.hpp"
#include "bna_player.hpp"
#include "bn_log.h"

bna::UI::UI() :
    _camera(bn::camera_ptr::create()) {
    _state = state::STOP;
    _phase = phase::STARTING;
}

void bna::UI::set_player(Player& player) {
    _ui_life.setCar(player.getCarRef());
    _ui_power.set_player(player);
    _player = &player;
    _previous_life = _player->getCarRef().getLife();
    _ui_life.set_camera(_camera);
    _ui_power.set_camera(_camera);
}

void bna::UI::update() {
    if (_player->getCarRef().getLife() < _previous_life) {
        _phase = phase::STARTING;
        _state = state::SHAKE;
        _previous_life = _player->getCarRef().getLife();
    }

    _ui_life.update();
    _ui_power.update();

    if (state::STOP == _state) {
        if (phase::STARTING == _phase) {
            _camera.set_position(0, 0);
            _phase = phase::READY;
        }
    }
    else if (state::SHAKE == _state) {
        _state = state::MOVE_RIGHT;
        _phase = phase::STARTING;
    }
    else if (state::MOVE_RIGHT == _state) {
        _camera.set_x(_camera.x() - 0.4);
        if (_camera.x() < -3) {
            _state = state::MOVE_LEFT;
            _phase = phase::STARTING;
        }
    }
    else if (state::MOVE_LEFT == _state) {
        _camera.set_x(_camera.x() + 0.4);
        if (_camera.x() >= 3) {
            _state = state::STOP;
            _phase = phase::STARTING;
        }
    }
}

void bna::UI::set_visible(bool visible) {
    _ui_life.set_visible(visible);
    _ui_power.set_visible(visible);
}