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

#define DEBUG
#ifdef DEBUG
#include "bn_log.h"
#endif


bna::CarSelection::CarSelection(CarBuilder& carBuilder,
    bn::array<parts::motors, 3>& motores, bn::array<parts::bodys, 3>& cuerpos, bn::array<parts::wheels, 3>& ruedas
) :
    _bodysIcon(bn::sprite_items::selection_icons_weight_light.create_sprite(0, 0)),
    _motorsIcon(bn::sprite_items::selection_icons_motor_weak.create_sprite(0, 0)),
    _wheelsIcon(bn::sprite_items::selection_icons_wheel_old.create_sprite(0, 0)),
    _background(bn::regular_bg_items::menu_parts.create_bg(0, 0)) {
    _carBuilder = &carBuilder;
    _continuar = false;
    _idOpcion = opcionesPartes(0);

    _motores = motores;
    _cuerpos = cuerpos;
    _ruedas = ruedas;

    constexpr int ALINEACION_HORIZONTAL = -48;
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

    constexpr int OFFSET_HORIZONTAL_TEXTO = 10;

    _textoStats = bna::TextManager(
        _indicadores[0].x(),
        _indicadores[0].y(),
        ""
    );
    _textoStats.set_aligment(bn::sprite_text_generator::alignment_type::CENTER);

    // _generateBodyRoulette();
    _textoCuerpo = bna::TextManager(
        _indicadores[2].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[2].y() + OFFSET_HORIZONTAL_TEXTO,
        ""
    );

    // _generateMotorRoulette();
    _textoMotor = bna::TextManager(
        _indicadores[1].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[1].y() + OFFSET_HORIZONTAL_TEXTO,
        ""
    );

    // _generateWheelRoulette();
    _textoRueda = bna::TextManager(
        _indicadores[3].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[3].y() + OFFSET_HORIZONTAL_TEXTO,
        ""
    );

    _textoPlay = bna::TextManager(
        _indicadores[4].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[4].y(),
        "Play"
    );
    _textoVolver = bna::TextManager(
        _indicadores[5].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[5].y(),
        "Back"
    );

    _idBody = bna::parts::bodys(0);
    _idMotor = bna::parts::motors(0);
    _idWheel = bna::parts::wheels(0);


    _updateStatsText();
    _updateBodyText();
    _updateMotorText();
    _updateWheelText();

    _puntero = bn::sprite_items::pointer.create_sprite(_indicadores[int(_idOpcion) + 1]);
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
    while (!_continuar) {
        if (bn::keypad::down_pressed()) {
            _idOpcion = opcionesPartes(bna::loop(int(_idOpcion) + 1, 0, int(opcionesPartes::VOLVER)));
            _puntero->set_position(_indicadores[int(_idOpcion) + 1]);
        }
        else if (bn::keypad::up_pressed()) {
            _idOpcion = opcionesPartes(bna::loop(int(_idOpcion) - 1, 0, int(opcionesPartes::VOLVER)));
            _puntero->set_position(_indicadores[int(_idOpcion) + 1]);
        }


        int cambio_opcion = int(bn::keypad::right_pressed()) - int(bn::keypad::left_pressed());
        if (_idOpcion == opcionesPartes::CUERPO) {
            if (cambio_opcion) {
                _idBody = bna::parts::bodys(bna::loop(int(_idBody) + cambio_opcion, 0, max_body));
                _updateBodyText();
                _updateStatsText();
                _updateBodyIcon();
            }
        }
        else if (_idOpcion == opcionesPartes::MOTOR) {
            if (cambio_opcion) {
                _idMotor = bna::parts::motors(bna::loop(int(_idMotor) + cambio_opcion, 0, max_motor));
                _updateMotorText();
                _updateStatsText();
                _updateMotorIcon();
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
                return bna::scene_type::CHARACTER_SELECTION;
            }
            else if (_idOpcion == opcionesPartes::PLAY) {
                if (_checkValidCombination()) {
                    // _wheelsRoulette.~IconRoulette();
                    // _bodysRoulette.~IconRoulette();
                    // _motorsRoulette.~IconRoulette();

                    _carBuilder->body = _idBody;
                    _carBuilder->motor = _idMotor;
                    _carBuilder->wheel = _idWheel;
                    return bna::scene_type::TEST_MAP;
                }
            }
        }
        if (bn::keypad::b_pressed()) {
            return bna::scene_type::CHARACTER_SELECTION;
        }
        if (bn::keypad::start_pressed()) {
            if (_checkValidCombination()) {
                _carBuilder->body = _idBody;
                _carBuilder->motor = _idMotor;
                _carBuilder->wheel = _idWheel;
                return bna::scene_type::TEST_MAP;
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

    _textoStats.updateText(
        texto,
        35
    );
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

    _textoCuerpo.updateText(texto);
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

    _textoMotor.updateText(texto);
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

    _textoRueda.updateText(texto);
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

// void bna::CarSelection::_generateBodyRoulette() {
//     bn::array<bn::sprite_ptr, 3> icons({ (_cuerpos[0] != bna::parts::bodys::EMPTY ? bn::sprite_items::cuerpos : bn::sprite_items::cuerpos_hide).create_sprite(0,0,0),
//             (_cuerpos[1] != bna::parts::bodys::EMPTY ? bn::sprite_items::cuerpos : bn::sprite_items::cuerpos_hide).create_sprite(0,0,1),
//             (_cuerpos[2] != bna::parts::bodys::EMPTY ? bn::sprite_items::cuerpos : bn::sprite_items::cuerpos_hide).create_sprite(0,0,2) });
//     // _bodysRoulette = bna::IconRoulette(
//     //     _indicadores[1], 20,
//     //     icons
//     // );
// }

// void bna::CarSelection::_generateMotorRoulette() {
//     bn::array<bn::sprite_ptr, 3> icons({
//             (_motores[0] != bna::parts::motors::EMPTY ? bn::sprite_items::motores : bn::sprite_items::motores_hide).create_sprite(0,0,0),
//             (_motores[1] != bna::parts::motors::EMPTY ? bn::sprite_items::motores : bn::sprite_items::motores_hide).create_sprite(0,0,1),
//             (_motores[2] != bna::parts::motors::EMPTY ? bn::sprite_items::motores : bn::sprite_items::motores_hide).create_sprite(0,0,2)
//         });
//     // _motorsRoulette = bna::IconRoulette(
//     //     _indicadores[2], 20,
//     //     icons
//     // );
// }

// void bna::CarSelection::_generateWheelRoulette() {
//     bn::array<bn::sprite_ptr, 3> icons({
//             (_ruedas[0] != bna::parts::wheels::EMPTY ? bn::sprite_items::ruedas : bn::sprite_items::ruedas_hide).create_sprite(0,0,0),
//             (_ruedas[1] != bna::parts::wheels::EMPTY ? bn::sprite_items::ruedas : bn::sprite_items::ruedas_hide).create_sprite(0,0,1),
//             (_ruedas[2] != bna::parts::wheels::EMPTY ? bn::sprite_items::ruedas : bn::sprite_items::ruedas_hide).create_sprite(0,0,2)
//         }
//     );
//     // _wheelsRoulette = bna::IconRoulette(_indicadores[3], 20,
//     //     icons
//     // );
// }