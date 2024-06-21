#include "bna_car_selection.hpp"
#include "bn_core.h"

#include "bn_sprite_items_pointer.h"

#include "bna_scene_type.hpp"

#include "bn_keypad.h"
#include "bna_loop_value.hpp"

#include "bn_string.h"
#include "bna_parts.hpp"
#include "bna_car_builder.hpp"

#ifdef DEBUG
#include "bn_log.h"
#endif


bna::CarSelection::CarSelection(CarBuilder& carBuilder) {
    _carBuilder = &carBuilder;
    _continuar = false;
    _idOpcion = opcionesPartes(0);

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

    _textoCuerpo = bna::TextManager(
        _indicadores[1].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[1].y(),
        ""
    );
    _textoMotor = bna::TextManager(
        _indicadores[2].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[2].y(),
        ""
    );
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
                _idBody = bna::parts::bodys(bna::loop(int(_idBody) + cambio_opcion, 0, int(bna::parts::bodys::MAX) - 1));
                _updateBodyText();
                _updateStatsText();
            }
        }
        else if (_idOpcion == opcionesPartes::MOTOR) {
            if (cambio_opcion) {
                _idMotor = bna::parts::motors(bna::loop(int(_idMotor) + cambio_opcion, 0, int(bna::parts::motors::MAX) - 1));
                _updateMotorText();
                _updateStatsText();
            }
        }
        else if (_idOpcion == opcionesPartes::WHEEL) {
            if (cambio_opcion) {
                _idWheel = bna::parts::wheels(bna::loop(int(_idWheel) + cambio_opcion, 0, int(bna::parts::wheels::MAX) - 1));
                _updateWheelText();
                _updateStatsText();
            }
        }


        if (bn::keypad::a_pressed()) {
            if (_idOpcion == opcionesPartes::VOLVER) {
                return bna::scene_type::MAIN_MENU;
            }
            if (_idOpcion == opcionesPartes::PLAY) {
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

    if (bna::parts::wheels::NORMAL == _idWheel) {
        texto.append("Normal");
    }
    if (bna::parts::wheels::SPIKES == _idWheel) {
        texto.append("Spikes");
    }

    _textoRueda.updateText(texto);
}

