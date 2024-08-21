#include "bna_car_selection.hpp"
#include "bn_core.h"

#include "bn_sprite_items_pointer.h"
#include "bn_sprite_items_cuerpos.h"
#include "bn_sprite_items_cuerpos_hide.h"
#include "bn_sprite_items_motores.h"
#include "bn_sprite_items_motores_hide.h"
#include "bn_sprite_items_ruedas.h"
#include "bn_sprite_items_ruedas_hide.h"

#include "bna_scene_type.hpp"

#include "bn_keypad.h"
#include "bna_loop_value.hpp"

#include "bn_string.h"
#include "bna_parts.hpp"
#include "bna_car_builder.hpp"
#include "bna_characters_id.hpp"


#include "bn_regular_bg_items_menu_parts.h"
#include "bn_sprite_items_selection_icons_wheel_old.h"
#include "bn_sprite_items_selection_icons_wheel_normal.h"
#include "bn_sprite_items_selection_icons_wheel_spike.h"

#include "bn_sprite_items_selection_icons_motor_weak.h"
#include "bn_sprite_items_selection_icons_motor_normal.h"
#include "bn_sprite_items_selection_icons_motor_fast.h"

#include "bn_sprite_items_selection_icons_weight_light.h"
#include "bn_sprite_items_selection_icons_weight_normal.h"
#include "bn_sprite_items_selection_icons_weight_heavy.h"

#include "bn_sprite_items_main_menu_back.h"
#include "bn_sprite_items_start_button.h"

#include "bn_regular_bg_items_cat_black_car.h"
#include "bn_regular_bg_items_cat_persian_car.h"
#include "bn_regular_bg_items_cat_siamese_car.h"
#include "bn_regular_bg_items_cat_tricolour_car.h"

#define DEBUG
#ifdef DEBUG
#include "bn_log.h"
#endif


bna::CarSelection::CarSelection(CarBuilder& carBuilder,
    bn::array<parts::motors, 3>& motores, bn::array<parts::bodys, 3>& cuerpos, bn::array<parts::wheels, 3>& ruedas,
    CharactersId& playerCharacter
) :
    _bodysIcon(bn::sprite_items::selection_icons_weight_light.create_sprite(0, 0)),
    _motorsIcon(bn::sprite_items::selection_icons_motor_weak.create_sprite(0, 0)),
    _wheelsIcon(bn::sprite_items::selection_icons_wheel_old.create_sprite(0, 0)),

    _backButton(bn::sprite_items::main_menu_back.create_sprite(-90, 30)),
    _playButton(bn::sprite_items::start_button.create_sprite(90, 30)),

    _background(bn::regular_bg_items::menu_parts.create_bg(0, 0)) {
    _character = playerCharacter;
    _carBuilder = &carBuilder;
    _continuar = false;
    _idOpcion = opcionesPartes(0);

    BN_LOG("char:", int(playerCharacter));
    if (bna::CharactersId::BLACK == playerCharacter) {
        _car = bn::regular_bg_items::cat_black_car.create_bg(-2, 37);
    }
    else if (bna::CharactersId::PERSIAN == playerCharacter) {
        _car = bn::regular_bg_items::cat_persian_car.create_bg(-2, 37);
    }
    else if (bna::CharactersId::SIAMESE == playerCharacter) {
        _car = bn::regular_bg_items::cat_siamese_car.create_bg(-2, 37);
    }
    else if (bna::CharactersId::TRICOLOUR == playerCharacter) {
        _car = bn::regular_bg_items::cat_tricolour_car.create_bg(-2, 37);
    }

    _motores = motores;
    _cuerpos = cuerpos;
    _ruedas = ruedas;

    constexpr int ALINEACION_HORIZONTAL = -49;
    constexpr bool MOSTRAR_INDICADORES = false;

    _indicadores.push_back(bna::Indicator(bn::fixed_point(0, -50), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(-74, ALINEACION_HORIZONTAL), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(4, ALINEACION_HORIZONTAL), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(82, ALINEACION_HORIZONTAL), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 60), MOSTRAR_INDICADORES));

    _motorsIcon.set_position(_indicadores[1]);
    _bodysIcon.set_position(_indicadores[2]);
    _wheelsIcon.set_position(_indicadores[3]);

    _idBody = bna::parts::bodys(0);
    _idMotor = bna::parts::motors(0);
    _idWheel = bna::parts::wheels(0);


    _updateStatsText();
    _updateBodyText();
    _updateMotorText();
    _updateWheelText();
}


bn::optional<bna::scene_type> bna::CarSelection::update() {
    bn::fixed brillo;
    int max_body = int(bna::parts::bodys::MAX) - 1;
    for (int i = 0; i < _cuerpos.size(); i++) {
        if (_cuerpos[i] == bna::parts::bodys::EMPTY) {
            max_body = i - 1;
            break;
        }
    }

    int max_motor = int(bna::parts::motors::MAX) - 1;
    for (int i = 0; i < _motores.size(); i++) {
        if (_motores[i] == bna::parts::motors::EMPTY) {
            max_motor = i - 1;
            break;
        }
    }
    int max_wheel = int(bna::parts::wheels::MAX) - 1;
    for (int i = 0; i < _motores.size(); i++) {
        if (_ruedas[i] == bna::parts::wheels::EMPTY) {
            max_wheel = i - 1;
            break;
        }
    }

    if (_idOpcion == opcionesPartes::CUERPO) {
        _updateBodyAnimation();
    }
    else if (_idOpcion == opcionesPartes::MOTOR) {
    }
    else if (_idOpcion == opcionesPartes::WHEEL) {
    }

    _updateSelectedLoopMovement();

    bool boton_presionado = false;
    while (!_continuar) {
        if (boton_presionado) {
            if (_animationPressedButton->done()) {
                if (opcionesPartes::PLAY == _idOpcion) {
                    return bna::scene_type::TEST_MAP;

                }
                else if (opcionesPartes::VOLVER == _idOpcion) {
                    return bna::scene_type::CHARACTER_SELECTION;
                }
            }
            _animationPressedButton->update();
        }
        else {
            if (_animationSelectedBody.has_value()) {
                if (!_animationSelectedBody->done()) {
                    _animationSelectedBody->update();
                }
            }
            if (_animationSelectedMotor.has_value()) {
                if (!_animationSelectedMotor->done()) {
                    _animationSelectedMotor->update();
                }
            }

            opcionesPartes opcionAnterior = _idOpcion;
            if (bn::keypad::left_pressed()) {
                if (opcionesPartes::PLAY == _idOpcion) {
                    _idOpcion = opcionesPartes::WHEEL;
                }
                else if (opcionesPartes::WHEEL == _idOpcion) {
                    _idOpcion = opcionesPartes::CUERPO;
                }
                else if (opcionesPartes::CUERPO == _idOpcion) {
                    _idOpcion = opcionesPartes::MOTOR;
                }
                else if (opcionesPartes::MOTOR == _idOpcion) {
                    _idOpcion = opcionesPartes::VOLVER;
                }
                else if (opcionesPartes::VOLVER == _idOpcion) {
                    _idOpcion = opcionesPartes::PLAY;
                }
                _updateSelectedLoopMovement();
            }
            else if (bn::keypad::right_pressed()) {
                if (opcionesPartes::VOLVER == _idOpcion) {
                    _idOpcion = opcionesPartes::MOTOR;
                }
                else if (opcionesPartes::MOTOR == _idOpcion) {
                    _idOpcion = opcionesPartes::CUERPO;
                }
                else if (opcionesPartes::CUERPO == _idOpcion) {
                    _idOpcion = opcionesPartes::WHEEL;
                }
                else if (opcionesPartes::WHEEL == _idOpcion) {
                    _idOpcion = opcionesPartes::PLAY;
                }
                else if (opcionesPartes::PLAY == _idOpcion) {
                    _idOpcion = opcionesPartes::VOLVER;
                }
                _updateSelectedLoopMovement();
            }


            int cambio_opcion = int(bn::keypad::up_pressed()) - int(bn::keypad::down_pressed());
            if (_idOpcion == opcionesPartes::CUERPO) {
                if (opcionAnterior != _idOpcion) {
                    _updateBodyAnimation();
                }
                if (cambio_opcion) {
                    _idBody = bna::parts::bodys(bna::loop(int(_idBody) + cambio_opcion, 0, max_body));
                    _updateBodyText();
                    _updateStatsText();
                    _updateBodyIcon();
                    _updateBodyAnimation();
                }
            }
            else if (_idOpcion == opcionesPartes::MOTOR) {
                if (opcionAnterior != _idOpcion) {
                    _updateMotorAnimation();
                }
                if (cambio_opcion) {
                    _idMotor = bna::parts::motors(bna::loop(int(_idMotor) + cambio_opcion, 0, max_motor));
                    _updateMotorText();
                    _updateStatsText();
                    _updateMotorIcon();
                    _updateMotorAnimation();
                }
            }
            else if (_idOpcion == opcionesPartes::WHEEL) {
                if (cambio_opcion) {
                    _idWheel = bna::parts::wheels(bna::loop(int(_idWheel) + cambio_opcion, 0, max_wheel));
                    _updateWheelText();
                    _updateStatsText();
                    _updateWheelIcon();
                }
            }


            if (bn::keypad::a_pressed()) {
                if (_idOpcion == opcionesPartes::VOLVER) {
                    _pressButton();
                    boton_presionado = true;
                }
                else if (_idOpcion == opcionesPartes::PLAY) {
                    _pressButton();
                    boton_presionado = true;
                    _carBuilder->body = _idBody;
                    _carBuilder->motor = _idMotor;
                    _carBuilder->wheel = _idWheel;
                }
            }
            if (bn::keypad::b_pressed()) {
                _idOpcion = opcionesPartes::VOLVER;
                _updateSelectedLoopMovement();
                _pressButton();
                boton_presionado = true;
            }
            if (bn::keypad::start_pressed()) {
                _carBuilder->body = _idBody;
                _carBuilder->motor = _idMotor;
                _carBuilder->wheel = _idWheel;
                _idOpcion = opcionesPartes::PLAY;
                _updateSelectedLoopMovement();
                _pressButton();
                boton_presionado = true;

            }
        }
        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

void bna::CarSelection::_updateBodyIcon() {
    if (bna::parts::bodys::LIGHT == _idBody) {
        _bodysIcon.set_item(bn::sprite_items::selection_icons_weight_light);
    }
    if (bna::parts::bodys::MEDIUM == _idBody) {
        _bodysIcon.set_item(bn::sprite_items::selection_icons_weight_normal);
    }
    if (bna::parts::bodys::HEAVY == _idBody) {
        _bodysIcon.set_item(bn::sprite_items::selection_icons_weight_heavy);
    }
}


void bna::CarSelection::_updateMotorIcon() {
    if (bna::parts::motors::SLOW == _idMotor) {
        _motorsIcon.set_item(bn::sprite_items::selection_icons_motor_weak);
    }
    if (bna::parts::motors::MEDIUM == _idMotor) {
        _motorsIcon.set_item(bn::sprite_items::selection_icons_motor_normal);
    }
    if (bna::parts::motors::FAST == _idMotor) {
        _motorsIcon.set_item(bn::sprite_items::selection_icons_motor_fast);
    }
}

void bna::CarSelection::_updateWheelIcon() {
    if (bna::parts::wheels::OLD == _idWheel) {
        _wheelsIcon.set_item(bn::sprite_items::selection_icons_wheel_old);
    }
    if (bna::parts::wheels::NORMAL == _idWheel) {
        _wheelsIcon.set_item(bn::sprite_items::selection_icons_wheel_normal);
    }
    if (bna::parts::wheels::SPIKES == _idWheel) {
        _wheelsIcon.set_item(bn::sprite_items::selection_icons_wheel_spike);
    }
}


void bna::CarSelection::_updateBodyAnimation() {
    if (bna::parts::bodys::LIGHT == _idBody) {
        _animationSelectedBody = bn::create_sprite_animate_action_once(_bodysIcon, 7, bn::sprite_items::selection_icons_weight_light.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8);
    }
    if (bna::parts::bodys::MEDIUM == _idBody) {
        _animationSelectedBody = bn::create_sprite_animate_action_once(_bodysIcon, 6, bn::sprite_items::selection_icons_weight_normal.tiles_item(), 0, 1, 2, 3, 4, 4);
    }
    if (bna::parts::bodys::HEAVY == _idBody) {
        _animationSelectedBody = bn::create_sprite_animate_action_once(_bodysIcon, 5, bn::sprite_items::selection_icons_weight_heavy.tiles_item(), 0, 1, 2, 3, 4, 4);
        _bodysIcon.set_item(bn::sprite_items::selection_icons_weight_heavy);
    }
}

void bna::CarSelection::_updateWheelAnimation() {
    // if (bna::parts::bodys::LIGHT == _idBody) {
    //     _animationSelectedBody = bn::create_sprite_animate_action_once(_bodysIcon, 7, bn::sprite_items::selection_icons_weight_light.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    // }
    // if (bna::parts::bodys::MEDIUM == _idBody) {
    //     _animationSelectedBody = bn::create_sprite_animate_action_once(_bodysIcon, 6, bn::sprite_items::selection_icons_weight_normal.tiles_item(), 0, 1, 2, 3, 4, 4);
    // }
    // if (bna::parts::bodys::HEAVY == _idBody) {
    //     _animationSelectedBody = bn::create_sprite_animate_action_once(_bodysIcon, 5, bn::sprite_items::selection_icons_weight_heavy.tiles_item(), 0, 1, 2, 3, 4, 4);
    //     _bodysIcon.set_item(bn::sprite_items::selection_icons_weight_heavy);
    // }
}

void bna::CarSelection::_updateMotorAnimation() {
    if (bna::parts::motors::SLOW == _idMotor) {
        _animationSelectedMotor = bn::create_sprite_animate_action_once(_motorsIcon, 7, bn::sprite_items::selection_icons_motor_weak.tiles_item(), 0, 1, 2, 0, 1, 2, 0, 0);
    }
    if (bna::parts::motors::MEDIUM == _idMotor) {
        _animationSelectedMotor = bn::create_sprite_animate_action_once(_motorsIcon, 7, bn::sprite_items::selection_icons_motor_normal.tiles_item(), 0, 1, 2, 0, 1, 2, 0, 0);
    }
    if (bna::parts::motors::FAST == _idMotor) {
        _animationSelectedMotor = bn::create_sprite_animate_action_once(_motorsIcon, 7, bn::sprite_items::selection_icons_motor_fast.tiles_item(), 0, 1, 2, 3, 0, 1, 2, 3, 0, 0);
    }
}

void bna::CarSelection::_updateSelectedLoopMovement() {
    _motorsIcon.set_position(_indicadores[1]);
    _bodysIcon.set_position(_indicadores[2]);
    _wheelsIcon.set_position(_indicadores[3]);
    _backButton.set_item(bn::sprite_items::main_menu_back);
    _playButton.set_item(bn::sprite_items::start_button);

    int offsetSeleccionado = -3;
    if (opcionesPartes::MOTOR == _idOpcion) {
        _motorsIcon.set_y(_indicadores[1].y() + offsetSeleccionado);
    }
    else if (opcionesPartes::CUERPO == _idOpcion) {
        _bodysIcon.set_y(_indicadores[2].y() + offsetSeleccionado);
    }
    else if (opcionesPartes::WHEEL == _idOpcion) {
        _wheelsIcon.set_y(_indicadores[3].y() + offsetSeleccionado);
    }
    else if (opcionesPartes::VOLVER == _idOpcion) {
        _backButton.set_item(bn::sprite_items::main_menu_back, 1);
    }
    else if (opcionesPartes::PLAY == _idOpcion) {
        _playButton.set_item(bn::sprite_items::start_button, 1);
    }
}

void bna::CarSelection::_pressButton() {
    int velocidad_parpadeo = 10;

    if (opcionesPartes::PLAY == _idOpcion) {
        _animationPressedButton = bn::create_sprite_animate_action_once(_playButton, velocidad_parpadeo, bn::sprite_items::start_button.tiles_item(), 0, 1, 0, 1, 0, 1);
    }
    else if (opcionesPartes::VOLVER == _idOpcion) {
        _animationPressedButton = bn::create_sprite_animate_action_once(_backButton, velocidad_parpadeo, bn::sprite_items::main_menu_back.tiles_item(), 0, 1, 0, 1, 0, 1);
    }
}

void bna::CarSelection::_updateStatsText() {
    bna::Stats stats;
    stats = stats + bna::parts::getBody(_idBody);
    stats = stats + bna::parts::getMotor(_idMotor);
    stats = stats + bna::parts::getWheels(_idWheel);

    bn::string<111> texto;
    texto.append("Speed: ");
    texto.append(bn::to_string<10>(stats.maxSpeed));
    texto.append(" Aceleration: ");
    texto.append(bn::to_string<10>(stats.aceleration));
    texto.append(" Turn: ");
    texto.append(bn::to_string<10>(stats.turn));
    texto.append(" Weight: ");
    texto.append(bn::to_string<10>(stats.weight));
}
void bna::CarSelection::_updateBodyText() {
    bn::string<111> texto = "Body: ";

    if (bna::parts::bodys::LIGHT == _idBody) {
        texto.append("Light");
    }
    if (bna::parts::bodys::MEDIUM == _idBody) {
        texto.append("Medium");
    }
    if (bna::parts::bodys::HEAVY == _idBody) {
        texto.append("Heavy");
    }
}
void bna::CarSelection::_updateMotorText() {
    bn::string<111> texto = "Motor: ";

    if (bna::parts::motors::SLOW == _idMotor) {
        texto.append("Slow");
    }
    if (bna::parts::motors::MEDIUM == _idMotor) {
        texto.append("Medium");
    }
    if (bna::parts::motors::FAST == _idMotor) {
        texto.append("Fast");
    }
}
void bna::CarSelection::_updateWheelText() {
    bn::string<111> texto = "Wheel: ";

    if (bna::parts::wheels::OLD == _idWheel) {
        texto.append("Old");
    }
    if (bna::parts::wheels::NORMAL == _idWheel) {
        texto.append("Normal");
    }
    if (bna::parts::wheels::SPIKES == _idWheel) {
        texto.append("Spikes");
    }
}

bool bna::CarSelection::_checkValidCombination() {
    if (_idBody != parts::bodys::EMPTY &&
        _idMotor != parts::motors::EMPTY &&
        _idWheel != parts::wheels::EMPTY
        ) {
        return true;
    }
    return false;
}
