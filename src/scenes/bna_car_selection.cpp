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

#ifdef DEBUG
#include "bn_log.h"
#endif



bna::CarSelection::CarSelection(CarBuilder& carBuilder,
    bn::array<parts::motors, 3>& motores, bn::array<parts::bodys, 3>& cuerpos, bn::array<parts::wheels, 3>& ruedas
) {
    _carBuilder = &carBuilder;
    _continuar = false;
    _idOpcion = opcionesPartes(0);

    _motores = motores;
    _cuerpos = cuerpos;
    _ruedas = ruedas;

    constexpr int ALINEACION_HORIZONTAL = -40;
    constexpr bool MOSTRAR_INDICADORES = false;

    _indicadores.push_back(bna::Indicator(bn::fixed_point(0, -50), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, -20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 0), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 60), MOSTRAR_INDICADORES));

    constexpr int OFFSET_HORIZONTAL_TEXTO = 10;

    _textoStats = bna::TextManager(
        _indicadores[0].x(),
        _indicadores[0].y(),
        ""
    );
    _textoStats.set_aligment(bn::sprite_text_generator::alignment_type::CENTER);

    _generateBodyRoulette();
    _textoCuerpo = bna::TextManager(
        _indicadores[1].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[1].y(),
        ""
    );

    _generateMotorRoulette();
    _textoMotor = bna::TextManager(
        _indicadores[2].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[2].y(),
        ""
    );

    _generateWheelRoulette();
    _textoRueda = bna::TextManager(
        _indicadores[3].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[3].y(),
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
                _bodysRoulette.changeOption(cambio_opcion);
                _idBody = _cuerpos[_bodysRoulette.getSelection()];
                _updateBodyText();
                _updateStatsText();
            }
        }
        else if (_idOpcion == opcionesPartes::MOTOR) {
            if (cambio_opcion) {
                _motorsRoulette.changeOption(cambio_opcion);
                _idMotor = _motores[_motorsRoulette.getSelection()];
                _updateMotorText();
                _updateStatsText();
            }
        }
        else if (_idOpcion == opcionesPartes::WHEEL) {
            if (cambio_opcion) {
                _wheelsRoulette.changeOption(cambio_opcion);
                _idWheel = _ruedas[_wheelsRoulette.getSelection()];
                _updateWheelText();
                _updateStatsText();
            }
        }


        if (bn::keypad::a_pressed()) {
            if (_idOpcion == opcionesPartes::VOLVER) {
                return bna::scene_type::CHARACTER_SELECTION;
            }
            if (_idOpcion == opcionesPartes::PLAY) {
                if (_checkValidCombination()) {
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

void bna::CarSelection::_generateBodyRoulette() {
    _bodysRoulette = bna::IconRoulette(
        _indicadores[1], 20,
        {
            (_cuerpos[0] != bna::parts::bodys::EMPTY ? bn::sprite_items::cuerpos : bn::sprite_items::cuerpos_hide).create_sprite(0,0,0),
            (_cuerpos[1] != bna::parts::bodys::EMPTY ? bn::sprite_items::cuerpos : bn::sprite_items::cuerpos_hide).create_sprite(0,0,1),
            (_cuerpos[2] != bna::parts::bodys::EMPTY ? bn::sprite_items::cuerpos : bn::sprite_items::cuerpos_hide).create_sprite(0,0,2)
        }
    );
}

void bna::CarSelection::_generateMotorRoulette() {
    _motorsRoulette = bna::IconRoulette(
        _indicadores[2], 20,
        {
            (_motores[0] != bna::parts::motors::EMPTY ? bn::sprite_items::motores : bn::sprite_items::motores_hide).create_sprite(0,0,0),
            (_motores[1] != bna::parts::motors::EMPTY ? bn::sprite_items::motores : bn::sprite_items::motores_hide).create_sprite(0,0,1),
            (_motores[2] != bna::parts::motors::EMPTY ? bn::sprite_items::motores : bn::sprite_items::motores_hide).create_sprite(0,0,2)
        }
    );
}

void bna::CarSelection::_generateWheelRoulette() {
    _wheelsRoulette = bna::IconRoulette(_indicadores[3], 20,
        {
            (_ruedas[0] != bna::parts::wheels::EMPTY ? bn::sprite_items::ruedas : bn::sprite_items::ruedas_hide).create_sprite(0,0,0),
            (_ruedas[1] != bna::parts::wheels::EMPTY ? bn::sprite_items::ruedas : bn::sprite_items::ruedas_hide).create_sprite(0,0,1),
            (_ruedas[2] != bna::parts::wheels::EMPTY ? bn::sprite_items::ruedas : bn::sprite_items::ruedas_hide).create_sprite(0,0,2)
        }
    );
}