#include "bna_character_selection.hpp"
#include "bn_core.h"

#include "bna_scene_type.hpp"

#include "bn_keypad.h"
#include "bna_loop_value.hpp"

#include "bn_string.h"
#include "bna_characters_id.hpp"
#include "bn_regular_bg_items_cat_gray.h"

#include "bn_sprite_items_icons_selection.h"

#include "bn_sprite_items_cat_selection_border.h"

#include "bn_sprite_items_cat_black_selection_icon.h"
#include "bn_sprite_items_cat_persian_selection_icon.h"
#include "bn_sprite_items_cat_siamese_selection_icon.h"
#include "bn_sprite_items_cat_tricolour_selection_icon.h"

#include "bn_sprite_items_cat_black_selection_body.h"
#include "bn_sprite_items_cat_persian_selection_body.h"
#include "bn_sprite_items_cat_siamese_selection_body.h"
#include "bn_sprite_items_cat_tricolour_selection_body.h"

#include "bn_regular_bg_items_character_selection.h"
#include "bn_sprite_items_main_menu_back.h"


#ifdef DEBUG
#include "bn_log.h"
#endif


bna::CharacterSelection::CharacterSelection(CharactersId& character) :
    _background(bn::regular_bg_items::character_selection.create_bg(0, 0)),
    _backButton(bn::sprite_items::main_menu_back.create_sprite(-104, 64)) {
    _character = &character;
    _continuar = false;
    _idOpcion = opcionesCharacter(0);

    constexpr int ALINEACION_HORIZONTAL = -84;
    constexpr bool MOSTRAR_INDICADORES = false;

    _indicadores.push_back(bna::Indicator(bn::fixed_point(0, -70), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, -28), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL + 64, -28), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 22), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL + 64, 22), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 60), MOSTRAR_INDICADORES));

    _iconosCharacters.push_back(bn::sprite_items::cat_black_selection_icon.create_sprite(_indicadores[1]));
    _iconosCharacters.push_back(bn::sprite_items::cat_persian_selection_icon.create_sprite(_indicadores[2]));
    _iconosCharacters.push_back(bn::sprite_items::cat_siamese_selection_icon.create_sprite(_indicadores[3]));
    _iconosCharacters.push_back(bn::sprite_items::cat_tricolour_selection_icon.create_sprite(_indicadores[4]));

    _seleccionado = false;
    _puntero = bn::sprite_items::cat_selection_border.create_sprite(_indicadores[int(_idOpcion) + 1]);
    _pointerAnimation = bn::create_sprite_animate_action_forever(_puntero.value(), 8, bn::sprite_items::cat_selection_border.tiles_item(), 0, 1, 2, 3);
    _updateCharacterSelected();
}


bn::optional<bna::scene_type> bna::CharacterSelection::update() {
    while (!_continuar) {
        if (_seleccionado) {
            if (_animationIcon->done()) {
                _updateCharacterPointer();
                if (_idOpcionSeleccionada == opcionesCharacter::BACK) {
                    return bna::scene_type::MAIN_MENU;
                }
                return bna::scene_type::CAR_SELECTION;
            }
            _animationIcon->update();
        }
        else {
            _pointerAnimation->update();
            _updateArrowPress();

            if (bn::keypad::a_pressed()) {
                _idOpcionSeleccionada = _idOpcion;
                _seleccionado = true;
                _puntero->set_visible(false);
                _updateCharacterSelected();
            }

            if (bn::keypad::b_pressed()) {
                _idOpcionSeleccionada = opcionesCharacter::BACK;
                _seleccionado = true;
                _updateCharacterSelected();
                _puntero->set_visible(false);
            }
            if (bn::keypad::start_pressed()) {
                if (_seleccionado) {
                    _updateCharacterPointer();
                    return bna::scene_type::CAR_SELECTION;
                }
            }
        }

        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

void bna::CharacterSelection::_updateArrowPress() {
    bn::fixed_point offset_puntero(0, 0);
    if (bn::keypad::down_pressed()) {
        if (int(_idOpcion) == 0 or int(_idOpcion) == 1) {
            _idOpcion = opcionesCharacter(int(_idOpcion) + 2);
        }
        else if (int(_idOpcion) == 2 or int(_idOpcion) == 3) {
            _idOpcion = opcionesCharacter::BACK;
        }
        else {
            _idOpcion = opcionesCharacter(bna::loop(int(_idOpcion) + 1, 0, int(opcionesCharacter::BACK)));
        }
        _puntero->set_position(_indicadores[int(_idOpcion) + 1] + offset_puntero);
        _updateCharacterSelected();
    }
    else if (bn::keypad::up_pressed()) {
        if (int(_idOpcion) == 0 or int(_idOpcion) == 1) {
            _idOpcion = opcionesCharacter::BACK;
        }
        else if (int(_idOpcion) == 2 or int(_idOpcion) == 3) {
            _idOpcion = opcionesCharacter(int(_idOpcion) - 2);
        }
        else {
            _idOpcion = opcionesCharacter(bna::loop(int(_idOpcion) - 1, 0, int(opcionesCharacter::BACK)));
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

    if (opcionesCharacter::BACK == _idOpcion) {
        _puntero->set_visible(false);
        _backButton.set_item(bn::sprite_items::main_menu_back, 1);
    }
    else {
        _puntero->set_visible(true);
        _backButton.set_item(bn::sprite_items::main_menu_back);
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
    _animateSelectedIcon();
}

void bna::CharacterSelection::_animateSelectedIcon() {
    if (opcionesCharacter::BLACK == _idOpcionSeleccionada) {
        _animationIcon = bn::create_sprite_animate_action_once(_iconosCharacters[0], 12, bn::sprite_items::cat_black_selection_icon.tiles_item(), 0, 1, 1, 1, 1, 1);
    }
    if (opcionesCharacter::PERSIAN == _idOpcionSeleccionada) {

        _animationIcon = bn::create_sprite_animate_action_once(_iconosCharacters[1], 8, bn::sprite_items::cat_persian_selection_icon.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6);
    }
    if (opcionesCharacter::SIAMESE == _idOpcionSeleccionada) {
        _animationIcon = bn::create_sprite_animate_action_once(_iconosCharacters[2], 8, bn::sprite_items::cat_siamese_selection_icon.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6);
    }
    if (opcionesCharacter::TRICOLOUR == _idOpcionSeleccionada) {
        _animationIcon = bn::create_sprite_animate_action_once(_iconosCharacters[3], 9, bn::sprite_items::cat_tricolour_selection_icon.tiles_item(), 0, 1, 2, 3, 4, 4, 4, 4, 4, 4);
    }
    if (opcionesCharacter::BACK == _idOpcionSeleccionada) {
        int velocidad_parpadeo = 10;
        _animationIcon = bn::create_sprite_animate_action_once(_backButton, velocidad_parpadeo, bn::sprite_items::main_menu_back.tiles_item(), 0, 1, 0, 1, 0, 1);
    }
}
