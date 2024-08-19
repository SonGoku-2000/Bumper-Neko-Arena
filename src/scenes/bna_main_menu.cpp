#include "bna_main_menu.hpp"
#include "bn_core.h"

#include "bna_scene_type.hpp"

#include "bn_sprite_items_pointer.h"
#include "bn_regular_bg_items_main_menu.h"

#include "bn_regular_bg_items_main_menu_play.h"
#include "bn_sprite_items_main_menu_options.h"
#include "bn_sprite_items_main_menu_back.h"
#include "bna_planes.hpp"

#include "bn_keypad.h"

#include "bna_loop_value.hpp"


#include "bn_log.h"

bna::MainMenu::MainMenu() :
    _playButton(bn::regular_bg_items::main_menu_play.create_bg(61, -8)),
    _optionsButton(bn::sprite_items::main_menu_options.create_sprite(68, 42)),
    _backButton(bn::sprite_items::main_menu_back.create_sprite(19, 45)),

    _background(bn::regular_bg_items::main_menu.create_bg(0, 0)),

    _animation(bn::create_regular_bg_animate_action_forever(_background, 25, bn::regular_bg_items::main_menu.map_item(), 0, 1, 2, 3, 4, 5)) {

    _background.set_priority(bna::Planes::BACKGROUND);

    _playButton.set_priority(bna::Planes::SECOND);

    _idOpcion = options::PLAY;

    constexpr int ALINEACION_HORIZONTAL = -40;
    constexpr bool MOSTRAR_INDICADORES = false;
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 0), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 60), MOSTRAR_INDICADORES));

    _updateSelectedOptionIcon();
}


bn::optional<bna::scene_type> bna::MainMenu::update() {
    bool button_pressed = false;
    while (true) {
        _animation.update();

        if (button_pressed) {
            if (_animationPlayButton.has_value()) {
                if (_animationPlayButton->done()) {
                    return bna::scene_type::CHARACTER_SELECTION;
                }
                _animationPlayButton->update();
            }

            if (_animationOptionBackButton.has_value()) {
                if (_animationOptionBackButton->done()) {
                    if (options::OPTIONS == _idOpcion) {
                        return bna::scene_type::OPTION_MENU;
                    }
                    if (options::BACK == _idOpcion) {
                        return bna::scene_type::TITLE_SCREEN;
                    }
                }
                _animationOptionBackButton->update();
            }
        }
        else {
            if (bn::keypad::down_pressed()) {
                if (options::PLAY == _idOpcion) {
                    _idOpcion = options::OPTIONS;
                }
                else if (options::OPTIONS == _idOpcion) {
                    _idOpcion = options::PLAY;
                }
                else if (options::BACK == _idOpcion) {
                    _idOpcion = options::PLAY;
                }
            }

            if (bn::keypad::up_pressed()) {
                if (options::PLAY == _idOpcion) {
                    _idOpcion = options::OPTIONS;
                }
                else  if (options::OPTIONS == _idOpcion) {
                    _idOpcion = options::PLAY;
                }
                else  if (options::BACK == _idOpcion) {
                    _idOpcion = options::PLAY;
                }
            }

            if (bn::keypad::left_pressed()) {
                if (options::PLAY == _idOpcion) {
                    _idOpcion = options::BACK;
                }
                else  if (options::OPTIONS == _idOpcion) {
                    _idOpcion = options::BACK;
                }
                else  if (options::BACK == _idOpcion) {
                    _idOpcion = options::OPTIONS;
                }
            }

            if (bn::keypad::right_pressed()) {
                if (options::PLAY == _idOpcion) {
                    _idOpcion = options::OPTIONS;
                }
                else  if (options::OPTIONS == _idOpcion) {
                    _idOpcion = options::BACK;
                }
                else  if (options::BACK == _idOpcion) {
                    _idOpcion = options::OPTIONS;
                }
            }

            if (bn::keypad::a_pressed()) {
                button_pressed = true;
                _animatePressedButton();
            }

            if (bn::keypad::b_pressed()) {
                button_pressed = true;
                _idOpcion = options::BACK;
                _animatePressedButton();
            }

            if (bn::keypad::start_pressed()) {
                button_pressed = true;
                _idOpcion = options::PLAY;
                _animatePressedButton();
            }

            if (bn::keypad::any_pressed()) {
                _updateSelectedOptionIcon();
            }
        }

        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

void bna::MainMenu::_updateSelectedOptionIcon() {
    _playButton.set_item(bn::regular_bg_items::main_menu_play, 0);
    _optionsButton.set_item(bn::sprite_items::main_menu_options, 0);
    _backButton.set_item(bn::sprite_items::main_menu_back, 0);

    if (options::PLAY == _idOpcion) {
        _playButton.set_item(bn::regular_bg_items::main_menu_play, 1);
    }
    if (options::OPTIONS == _idOpcion) {
        _optionsButton.set_item(bn::sprite_items::main_menu_options, 1);
    }
    if (options::BACK == _idOpcion) {
        _backButton.set_item(bn::sprite_items::main_menu_back, 1);
    }
}

void bna::MainMenu::_animatePressedButton() {
    _playButton.set_item(bn::regular_bg_items::main_menu_play, 0);
    _optionsButton.set_item(bn::sprite_items::main_menu_options, 0);
    _backButton.set_item(bn::sprite_items::main_menu_back, 0);

    int velocidad_parpadeo = 10;
    if (options::PLAY == _idOpcion) {
        _animationPlayButton = bn::create_regular_bg_animate_action_once(_playButton, velocidad_parpadeo, bn::regular_bg_items::main_menu_play.map_item(), 0, 1, 0, 1, 0, 1, 0, 1, 0, 1);
    }
    if (options::OPTIONS == _idOpcion) {
        _animationOptionBackButton = bn::create_sprite_animate_action_once(_optionsButton, velocidad_parpadeo, bn::sprite_items::main_menu_options.tiles_item(), 0, 1, 0, 1, 0, 1, 0, 1, 0, 1);
    }
    if (options::BACK == _idOpcion) {
        _animationOptionBackButton = bn::create_sprite_animate_action_once(_backButton, velocidad_parpadeo, bn::sprite_items::main_menu_back.tiles_item(), 0, 1, 0, 1, 0, 1);
    }
}
