#include "bna_character_selection.hpp"
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


bna::CharacterSelection::CharacterSelection(CarBuilder& carBuilder) {
    _carBuilder = &carBuilder;
    _continuar = false;
    _idOpcion = opcionesCharacter(0);

    constexpr int ALINEACION_HORIZONTAL = -40;
    constexpr bool MOSTRAR_INDICADORES = false;

    _indicadores.push_back(bna::Indicator(bn::fixed_point(0, -70), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, -40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, -20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 0), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 60), MOSTRAR_INDICADORES));

    constexpr int OFFSET_HORIZONTAL_TEXTO = 10;

    _textoCharacterSeleccionado = bna::TextManager(
        _indicadores[0].x(),
        _indicadores[0].y(),
        ""
    );
    _textoCharacterSeleccionado.set_aligment(bn::sprite_text_generator::alignment_type::CENTER);

    _textoCharacter1 = bna::TextManager(
        _indicadores[1].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[1].y(),
        "Character 1"
    );
    _textoCharacter2 = bna::TextManager(
        _indicadores[2].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[2].y(),
        "Character 2"
    );
    _textoCharacter3 = bna::TextManager(
        _indicadores[3].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[3].y(),
        "Character 3"
    );
    _textoCharacter4 = bna::TextManager(
        _indicadores[4].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[4].y(),
        "Character 4"
    );
    _textoNext = bna::TextManager(
        _indicadores[5].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[5].y(),
        "Next"
    );
    _textoVolver = bna::TextManager(
        _indicadores[6].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[6].y(),
        "Back"
    );

    _seleccionado = false;
    _puntero = bn::sprite_items::pointer.create_sprite(_indicadores[int(_idOpcion) + 1]);
    _updateCharacterSelected();
}


bn::optional<bna::scene_type> bna::CharacterSelection::update() {
    bn::fixed brillo;
    while (!_continuar) {
        if (bn::keypad::down_pressed()) {
            _idOpcion = opcionesCharacter(bna::loop(int(_idOpcion) + 1, 0, int(opcionesCharacter::VOLVER)));
            _puntero->set_position(_indicadores[int(_idOpcion) + 1]);
            _updateCharacterSelected();
        }
        else if (bn::keypad::up_pressed()) {
            _idOpcion = opcionesCharacter(bna::loop(int(_idOpcion) - 1, 0, int(opcionesCharacter::VOLVER)));
            _puntero->set_position(_indicadores[int(_idOpcion) + 1]);
            _updateCharacterSelected();
        }


        // int cambio_opcion = int(bn::keypad::right_pressed()) - int(bn::keypad::left_pressed());
        // if (_idOpcion == opcionesCharacter::CUERPO) {
        //     if (cambio_opcion) {
        //         _idBody = bna::parts::bodys(bna::loop(int(_idBody) + cambio_opcion, 0, int(bna::parts::bodys::MAX) - 1));
        //         _updateBodyText();
        //         _updateStatsText();
        //     }
        // }
        // else if (_idOpcion == opcionesCharacter::MOTOR) {
        //     if (cambio_opcion) {
        //         _idMotor = bna::parts::motors(bna::loop(int(_idMotor) + cambio_opcion, 0, int(bna::parts::motors::MAX) - 1));
        //         _updateMotorText();
        //         _updateStatsText();
        //     }
        // }
        // else if (_idOpcion == opcionesCharacter::WHEEL) {
        //     if (cambio_opcion) {
        //         _idWheel = bna::parts::wheels(bna::loop(int(_idWheel) + cambio_opcion, 0, int(bna::parts::wheels::MAX) - 1));
        //         _updateWheelText();
        //         _updateStatsText();
        //     }
        // }


        if (bn::keypad::a_pressed()) {
            if (_idOpcion == opcionesCharacter::VOLVER) {
                return bna::scene_type::MAIN_MENU;
            }
            if (_idOpcion == opcionesCharacter::NEXT) {
                if (_seleccionado == true) {
                    return bna::scene_type::CAR_SELECTION;
                }
            }
            else {
                _idOpcionSeleccionada = _idOpcion;
                _seleccionado = true;
                _updateCharacterSelected();
            }
        }
        // if (bn::keypad::b_pressed()) {
        //     return bna::scene_type::MAIN_MENU;
        // }
        // if (bn::keypad::start_pressed()) {
        //     _carBuilder->body = _idBody;
        //     _carBuilder->motor = _idMotor;
        //     _carBuilder->wheel = _idWheel;
        //     return bna::scene_type::TEST_MAP;
        // }

        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

void bna::CharacterSelection::_updateCharacterSelected() {
    if (_seleccionado == true) {
        if (opcionesCharacter::CAT_1 == _idOpcionSeleccionada) {
            _textoCharacterSeleccionado.updateText("Cat 1");
        }
        if (opcionesCharacter::CAT_2 == _idOpcionSeleccionada) {
            _textoCharacterSeleccionado.updateText("Cat 2");
        }
        if (opcionesCharacter::CAT_3 == _idOpcionSeleccionada) {
            _textoCharacterSeleccionado.updateText("Cat 3");
        }
        if (opcionesCharacter::BIRD == _idOpcionSeleccionada) {
            _textoCharacterSeleccionado.updateText("Cat Bird");
        }
        return;
    }

    if (opcionesCharacter::CAT_1 == _idOpcion) {
        _textoCharacterSeleccionado.updateText("Cat 1");
    }
    if (opcionesCharacter::CAT_2 == _idOpcion) {
        _textoCharacterSeleccionado.updateText("Cat 2");
    }
    if (opcionesCharacter::CAT_3 == _idOpcion) {
        _textoCharacterSeleccionado.updateText("Cat 3");
    }
    if (opcionesCharacter::BIRD == _idOpcion) {
        _textoCharacterSeleccionado.updateText("Cat Bird");
    }
    if (opcionesCharacter::NEXT == _idOpcion) {
        _textoCharacterSeleccionado.updateText("");
    }
    if (opcionesCharacter::VOLVER == _idOpcion) {
        _textoCharacterSeleccionado.updateText("");
    }
}

