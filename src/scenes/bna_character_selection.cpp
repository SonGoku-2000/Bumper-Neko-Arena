#include "bna_character_selection.hpp"
#include "bn_core.h"

#include "bn_sprite_items_pointer.h"

#include "bna_scene_type.hpp"

#include "bn_keypad.h"
#include "bna_loop_value.hpp"

#include "bn_string.h"
#include "bna_characters_id.hpp"
#include "bn_regular_bg_items_cat_gray.h"
#include "bn_sprite_items_icons_selection.h"

#include "bn_sprite_items_cat_black_selection_iconn.h"
#include "bn_sprite_items_cat_persian_selection_icon.h"
#include "bn_sprite_items_cat_siamese_selection_icon.h"
#include "bn_sprite_items_cat_tricolour_selection_icon.h"

#ifdef DEBUG
#include "bn_log.h"
#endif


bna::CharacterSelection::CharacterSelection(CharactersId& character) {
    _character = &character;
    _continuar = false;
    _idOpcion = opcionesCharacter(0);

    constexpr int ALINEACION_HORIZONTAL = -90;
    constexpr bool MOSTRAR_INDICADORES = false;

    _indicadores.push_back(bna::Indicator(bn::fixed_point(0, -70), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, -40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL + 64, -40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 10), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL + 64, 10), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 60), MOSTRAR_INDICADORES));

    constexpr int OFFSET_HORIZONTAL_TEXTO = 10;

    _textoCharacterSeleccionado = bna::TextManager(
        _indicadores[0].x(),
        _indicadores[0].y(),
        ""
    );
    _textoCharacterSeleccionado.set_aligment(bn::sprite_text_generator::alignment_type::CENTER);

    _iconosCharacters.push_back(bn::sprite_items::cat_black_selection_iconn.create_sprite(_indicadores[1]));
    _iconosCharacters.push_back(bn::sprite_items::cat_persian_selection_icon.create_sprite(_indicadores[2]));
    _iconosCharacters.push_back(bn::sprite_items::cat_siamese_selection_icon.create_sprite(_indicadores[3]));
    _iconosCharacters.push_back(bn::sprite_items::cat_tricolour_selection_icon.create_sprite(_indicadores[4]));

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
        _updateArrowPress();

        if (bn::keypad::a_pressed()) {
            if (_idOpcion == opcionesCharacter::VOLVER) {
                return bna::scene_type::MAIN_MENU;
            }
            if (_idOpcion == opcionesCharacter::NEXT) {
                if (_seleccionado == true) {
                    _updateCharacterPointer();
                    return bna::scene_type::CAR_SELECTION;
                }
            }
            else {
                _idOpcionSeleccionada = _idOpcion;
                _seleccionado = true;
                _updateCharacterSelected();
            }
        }

        if (bn::keypad::b_pressed()) {
            if (_seleccionado == true) {
                _seleccionado = false;
                _updateCharacterSelected();
            }
            else {
                return bna::scene_type::MAIN_MENU;
            }
        }
        if (bn::keypad::start_pressed()) {
            if (_seleccionado) {
                return bna::scene_type::CAR_SELECTION;
            }
        }

        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

void bna::CharacterSelection::_updateArrowPress() {
    if (bn::keypad::down_pressed()) {
        if (int(_idOpcion) == 0 or int(_idOpcion) == 1) {
            _idOpcion = opcionesCharacter(int(_idOpcion) + 2);
        }
        else if (int(_idOpcion) == 2 or int(_idOpcion) == 3) {
            _idOpcion = opcionesCharacter::NEXT;
        }
        else {
            _idOpcion = opcionesCharacter(bna::loop(int(_idOpcion) + 1, 0, int(opcionesCharacter::VOLVER)));
        }
        _puntero->set_position(_indicadores[int(_idOpcion) + 1]);
        _updateCharacterSelected();
    }
    else if (bn::keypad::up_pressed()) {
        if (int(_idOpcion) == 0 or int(_idOpcion) == 1) {
            _idOpcion = opcionesCharacter::VOLVER;
        }
        else if (int(_idOpcion) == 2 or int(_idOpcion) == 3) {
            _idOpcion = opcionesCharacter(int(_idOpcion) - 2);
        }
        else {
            _idOpcion = opcionesCharacter(bna::loop(int(_idOpcion) - 1, 0, int(opcionesCharacter::VOLVER)));
        }
        _puntero->set_position(_indicadores[int(_idOpcion) + 1]);
        _updateCharacterSelected();
    }
    else if (bn::keypad::right_pressed()) {
        if (int(_idOpcion) == 0 or int(_idOpcion) == 1) {
            _idOpcion = opcionesCharacter(bna::loop(int(_idOpcion) + 1, 0, 1));
        }
        else if (int(_idOpcion) == 2 or int(_idOpcion) == 3) {
            _idOpcion = opcionesCharacter(bna::loop(int(_idOpcion) + 1, 2, 3));
        }
        _puntero->set_position(_indicadores[int(_idOpcion) + 1]);
        _updateCharacterSelected();
    }
    else if (bn::keypad::left_pressed()) {
        if (int(_idOpcion) == 0 or int(_idOpcion) == 1) {
            _idOpcion = opcionesCharacter(bna::loop(int(_idOpcion) - 1, 0, 1));
        }
        else if (int(_idOpcion) == 2 or int(_idOpcion) == 3) {
            _idOpcion = opcionesCharacter(bna::loop(int(_idOpcion) - 1, 2, 3));
        }
        _puntero->set_position(_indicadores[int(_idOpcion) + 1]);
        _updateCharacterSelected();
    }
}

void bna::CharacterSelection::_updateCharacterPointer() {
    if (opcionesCharacter::BLACK == _idOpcionSeleccionada) {
        *_character = CharactersId::BLACK;
    }
    if (opcionesCharacter::PERSIAN == _idOpcionSeleccionada) {
        *_character = CharactersId::PERSIAN;
    }
    if (opcionesCharacter::SIAMESE == _idOpcionSeleccionada) {
        *_character = CharactersId::SIAMESE;
    }
    if (opcionesCharacter::TRICOLOUR == _idOpcionSeleccionada) {
        *_character = CharactersId::TRICOLOUR;
    }
}

void bna::CharacterSelection::_updateCharacterSelected() {
    if (_seleccionado == true) {
        if (opcionesCharacter::BLACK == _idOpcionSeleccionada) {
            _textoCharacterSeleccionado.updateText("Cat 1");
            _character_image = bn::regular_bg_items::cat_gray.create_bg(80, 0);
        }
        if (opcionesCharacter::PERSIAN == _idOpcionSeleccionada) {
            _textoCharacterSeleccionado.updateText("Cat 2");
        }
        if (opcionesCharacter::SIAMESE == _idOpcionSeleccionada) {
            _textoCharacterSeleccionado.updateText("Cat 3");
        }
        if (opcionesCharacter::VOLVER == _idOpcionSeleccionada) {
            _textoCharacterSeleccionado.updateText("Cat Bird");
        }
        return;
    }

    if (opcionesCharacter::BLACK == _idOpcion) {
        _textoCharacterSeleccionado.updateText("Cat 1");
    }
    if (opcionesCharacter::PERSIAN == _idOpcion) {
        _textoCharacterSeleccionado.updateText("Cat 2");
    }
    if (opcionesCharacter::SIAMESE == _idOpcion) {
        _textoCharacterSeleccionado.updateText("Cat 3");
    }
    if (opcionesCharacter::TRICOLOUR == _idOpcion) {
        _textoCharacterSeleccionado.updateText("Cat Bird");
    }
    if (opcionesCharacter::NEXT == _idOpcion) {
        _textoCharacterSeleccionado.updateText("");
    }
    if (opcionesCharacter::VOLVER == _idOpcion) {
        _textoCharacterSeleccionado.updateText("");
    }
}

