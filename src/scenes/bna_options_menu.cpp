#include "bna_options_menu.hpp"
#include "bn_core.h"

#include "bna_brightness_manager.hpp"
#include "bn_sprite_items_pointer.h"

#include "bna_scene_type.hpp"

#include "bn_keypad.h"
#include "bna_loop_value.hpp"

#include "bn_string.h"

#include "bn_regular_bg_items_menu_options.h"
#include "bn_sprite_items_menu_options_arrow.h"
#include "bn_sprite_items_menu_options_active_point.h"


bna::OptionsMenu::OptionsMenu(bn::fixed& brillo_memory) :
    _brillo(brillo_memory),
    _background(bn::regular_bg_items::menu_options.create_bg(0, 0)) {
    _continuar = false;
    _idOpcion = 0;

    constexpr int ALINEACION_HORIZONTAL = -40;
    constexpr bool MOSTRAR_INDICADORES = false;
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 0), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 20), MOSTRAR_INDICADORES));

    constexpr int OFFSET_HORIZONTAL_TEXTO = 10;
    _textoBrillo = bna::TextManager(
        _indicadores[0].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[0].y(),
        "Brightness: "
    );

    _updateBrightnessText();

    _textoVolver = bna::TextManager(
        _indicadores[1].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[1].y(),
        "Back"
    );

    _puntero = bn::sprite_items::pointer.create_sprite(_indicadores[_idOpcion]);

    _brightnessArrows.push_back(bn::sprite_items::menu_options_arrow.create_sprite(-77, 7));
    _brightnessArrows.push_back(bn::sprite_items::menu_options_arrow.create_sprite(52, 7, 1));

    for (int i = 0; i < _brightnessPoints.max_size(); i++) {
        _brightnessPoints.push_back(bn::sprite_items::menu_options_active_point.create_sprite(-60 + (16 * i), 6));
    }

    _updateBrightnessPoints();

    _volumeArrows.push_back(bn::sprite_items::menu_options_arrow.create_sprite(-77, 44));
    _volumeArrows.push_back(bn::sprite_items::menu_options_arrow.create_sprite(52, 44, 1));

    for (int i = 0; i < _volumePoints.max_size(); i++) {
        _volumePoints.push_back(bn::sprite_items::menu_options_active_point.create_sprite(-60 + (16 * i), 43));
    }

    _updateVolumePoints();
}


bn::optional<bna::scene_type> bna::OptionsMenu::update() {
    bn::fixed brillo;
    while (!_continuar) {
        if (bn::keypad::down_pressed()) {
            _idOpcion = bna::loop(_idOpcion + 1, 0, _indicadores.size() - 1);
            _puntero->set_position(_indicadores[_idOpcion]);
        }
        else if (bn::keypad::up_pressed()) {
            _idOpcion = bna::loop(_idOpcion - 1, 0, _indicadores.size() - 1);
            _puntero->set_position(_indicadores[_idOpcion]);
        }


        if (_idOpcion == 0) {
            brillo = bna::brightness_manager::get_brightness();
            constexpr bn::fixed CAMBIO_BRILLO = bn::fixed(0.005);
            if (bn::keypad::right_held()) {
                bna::brightness_manager::set_brightness(brillo + CAMBIO_BRILLO);
                _updateBrightnessText();
                _updateBrightnessPoints();
            }
            else if (bn::keypad::left_held()) {
                bna::brightness_manager::set_brightness(brillo - CAMBIO_BRILLO);
                _updateBrightnessPoints();
                _updateBrightnessText();
            }
        }


        if (bn::keypad::a_pressed()) {
            if (_idOpcion == 1) {
                _brillo = brillo;
                return bna::scene_type::MAIN_MENU;
            }
        }

        if (bn::keypad::b_pressed()) {
            _brillo = brillo;
            return bna::scene_type::MAIN_MENU;
        }
        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

void bna::OptionsMenu::_updateBrightnessText() {
    _textoBrillo.updateText("Brightness: " + bn::to_string<30>(bna::brightness_manager::get_brightness_percent()));
}

void bna::OptionsMenu::_updateBrightnessPoints() {
    int puntos_activos = bna::brightness_manager::get_brightness_percent() / 14;
    for (int i = 0; i < _brightnessPoints.size(); i++) {
        if (i < puntos_activos) {
            _brightnessPoints[i].set_visible(true);
        }
        else {
            _brightnessPoints[i].set_visible(false);
        }
    }
}

void bna::OptionsMenu::_updateVolumePoints() {

}

