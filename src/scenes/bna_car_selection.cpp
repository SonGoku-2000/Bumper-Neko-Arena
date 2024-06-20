#include "bna_car_selection.hpp"
#include "bn_core.h"

#include "bna_brightness_manager.hpp"
#include "bn_sprite_items_pointer.h"

#include "bna_scene_type.hpp"

#include "bn_keypad.h"
#include "bna_loop_value.hpp"

#include "bn_string.h"
#include "bn_log.h"
#include "bna_parts.hpp"

bna::CarSelection::CarSelection() {
    _continuar = false;
    _idOpcion = 1;

    constexpr int ALINEACION_HORIZONTAL = -40;
    constexpr bool MOSTRAR_INDICADORES = true;

    _indicadores.push_back(bna::Indicator(bn::fixed_point(0, -50), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, -20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 0), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 40), MOSTRAR_INDICADORES));

    constexpr int OFFSET_HORIZONTAL_TEXTO = 10;
    // _textoBrillo = bna::TextManager(
    //     _indicadores[0].x() + OFFSET_HORIZONTAL_TEXTO,
    //     _indicadores[0].y(),
    //     "Brightness: "
    // );

    // _updateBrightnessText();
    _textoStats = bna::TextManager(
        _indicadores[0].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[0].y(),
        ""
    );
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
    _textoVolver = bna::TextManager(
        _indicadores[4].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[4].y(),
        "Back"
    );

    _idBody = bna::parts::BODYS(0);
    _idMotor = bna::parts::MOTORS(0);
    _idWheel = bna::parts::WHEELS(0);


    _updateStatsText();
    _updateBodyText();
    _updateMotorText();
    _updateWheelText();

    _puntero = bn::sprite_items::pointer.create_sprite(_indicadores[_idOpcion]);
}


bn::optional<bna::scene_type> bna::CarSelection::update() {
    bn::fixed brillo;
    while (!_continuar) {
        if (bn::keypad::down_pressed()) {
            _idOpcion = bna::loop(_idOpcion + 1, 1, _indicadores.size() - 1);
            _puntero->set_position(_indicadores[_idOpcion]);
        }
        else if (bn::keypad::up_pressed()) {
            _idOpcion = bna::loop(_idOpcion - 1, 1, _indicadores.size() - 1);
            _puntero->set_position(_indicadores[_idOpcion]);
        }


        if (_idOpcion == 0) {

        }


        if (bn::keypad::a_pressed()) {
            if (_idOpcion == 4) {
                return bna::scene_type::MAIN_MENU;
            }
        }
        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

// void bna::CarSelection::_updateBrightnessText() {
//     _textoBrillo.updateText("Brightness: " + bn::to_string<30>(bna::brightness_manager::get_brightness_percent()));
// }

void bna::CarSelection::_updateStatsText() {
    _textoStats.set_aligment(bn::sprite_text_generator::alignment_type::CENTER);
    bn::string<111> texto;
    texto.append("Speed: ");
    texto.append("Aceleration: ");
    texto.append("Turn: ");
    texto.append("Weight: ");
    _textoStats.updateText(
        texto,
        25
        // ,
        //,
        // 
    );
}
void bna::CarSelection::_updateBodyText() {
    bn::string<111> texto = "Body: ";

    if (bna::parts::BODYS::LIGHT == _idBody) {
        texto.append("Light");
    }
    if (bna::parts::BODYS::MEDIUM == _idBody) {
        texto.append("Medium");
    }
    if (bna::parts::BODYS::HEAVY == _idBody) {
        texto.append("Heavy");
    }

    _textoCuerpo.updateText(texto);
}
void bna::CarSelection::_updateMotorText() {
    bn::string<111> texto = "Motor: ";

    if (bna::parts::MOTORS::SLOW == _idMotor) {
        texto.append("Slow");
    }
    if (bna::parts::MOTORS::MEDIUM == _idMotor) {
        texto.append("Medium");
    }
    if (bna::parts::MOTORS::FAST == _idMotor) {
        texto.append("Fast");
    }

    _textoMotor.updateText(texto);
}
void bna::CarSelection::_updateWheelText() {
    bn::string<111> texto = "Wheel: ";

    if (bna::parts::WHEELS::NORMAL == _idWheel) {
        texto.append("Normal");
    }
    if (bna::parts::WHEELS::SPIKES == _idWheel) {
        texto.append("Spikes");
    }

    _textoRueda.updateText(texto);
}

