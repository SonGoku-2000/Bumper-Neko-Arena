#include "bna_scene_win.hpp"
#include "bna_scene_type.hpp"
#include "bn_core.h"
#include "bna_parts.hpp"
#include "bn_log.h"
#include "bn_keypad.h"
bna::SceneWin::SceneWin(bn::array<parts::motors, 3>& motores, bn::array<parts::bodys, 3>& cuerpos, bn::array<parts::wheels, 3>& ruedas) {
    _motores = &motores;
    _cuerpos = &cuerpos;
    _ruedas = &ruedas;
    _texto_win.updateText("¡Win!");
    _texto_continue.set_y(30);
    _texto_continue.updateText("start to continue");
    _indicadoresOpciones.push_back(bna::Indicator(-50, 0, true, 2));
}

[[nodiscard]] bn::optional<bna::scene_type> bna::SceneWin::update() {
    _checkUnlockable();

    while (true) {
        if (bn::keypad::start_pressed()) {
            return bna::scene_type::MAIN_MENU;
        }
        bn::core::update();
    }
    return bna::scene_type::MAIN_MENU;
}

void bna::SceneWin::_checkUnlockable() {
    while (true) {
        if (!_aviableBodys() && !_aviableMotors() && !_aviableWheels()) {
            break;
        }
        int idObjeto = _random.get_int(3);
        if (idObjeto == 0) {
            if (_aviableMotors()) {
                for (int i = 0; i < _motores->size(); i++) {
                    if (_motores->at(i) == bna::parts::motors::EMPTY) {
                        _motores->at(i) = bna::parts::motors(i);
                        break;
                    }
                }
                break;
            }
        }
        if (idObjeto == 1) {
            if (_aviableBodys()) {
                for (int i = 0; i < _cuerpos->size(); i++) {
                    if (_cuerpos->at(i) == bna::parts::bodys::EMPTY) {
                        _cuerpos->at(i) = bna::parts::bodys(i);
                        break;
                    }
                }
                break;
            }
        }
        if (idObjeto == 2) {
            if (_aviableWheels()) {
                for (int i = 0; i < _ruedas->size(); i++) {
                    if (_ruedas->at(i) == bna::parts::wheels::EMPTY) {
                        _ruedas->at(i) = bna::parts::wheels(i);
                        break;
                    }
                }
                break;
            }
        }
    }
}

bool bna::SceneWin::_aviableBodys() {
    for (int i = 0; i < _cuerpos->size(); i++) {
        if (_cuerpos->at(i) == bna::parts::bodys::EMPTY) {
            return true;
        }
    }
    return false;
}

bool bna::SceneWin::_aviableMotors() {
    for (int i = 0; i < _motores->size(); i++) {
        if (_motores->at(i) == bna::parts::motors::EMPTY) {
            return true;
        }
    }
    return false;
}


bool bna::SceneWin::_aviableWheels() {
    for (int i = 0; i < _ruedas->size(); i++) {
        if (_ruedas->at(i) == bna::parts::wheels::EMPTY) {
            return true;
        }
    }
    return false;
}
