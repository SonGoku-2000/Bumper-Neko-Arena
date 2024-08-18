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

#include "bn_sound.h"


bna::OptionsMenu::OptionsMenu(bn::fixed& brillo_memory, bn::fixed& volume) :
    _brillo(brillo_memory),
    _volumen(volume),
    _background(bn::regular_bg_items::menu_options.create_bg(0, 0)) {
    _continuar = false;
    _idOpcion = 0;

    _framesTranscurridos = 0;
    _moveUp = false;

    constexpr int ALINEACION_HORIZONTAL = -40;
    constexpr bool MOSTRAR_INDICADORES = false;
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 0), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 20), MOSTRAR_INDICADORES));

    // constexpr int OFFSET_HORIZONTAL_TEXTO = 10;
    // // _textoBrillo = bna::TextManager(
    // //     _indicadores[0].x() + OFFSET_HORIZONTAL_TEXTO,
    // //     _indicadores[0].y(),
    // //     "Brightness: "
    // // );

    // _updateBrightnessText();

    // _textoVolver = bna::TextManager(
    //     _indicadores[1].x() + OFFSET_HORIZONTAL_TEXTO,
    //     _indicadores[1].y(),
    //     "Back"
    // );

    _puntero = bn::sprite_items::pointer.create_sprite(_indicadores[_idOpcion]);
    _puntero->set_visible(false);

    _brightnessArrows.push_back(bn::sprite_items::menu_options_arrow.create_sprite(-77, 7));
    _brightnessArrows.push_back(bn::sprite_items::menu_options_arrow.create_sprite(52, 7, 1));

    for (int i = 0; i < _brightnessPoints.max_size(); i++) {
        _brightnessPoints.push_back(bn::sprite_items::menu_options_active_point.create_sprite(-60 + (16 * i), 6));
    }

    _updateBrightnessPoints();

    _volumeArrows.push_back(bn::sprite_items::menu_options_arrow.create_sprite(-77, 44));
    _volumeArrows.push_back(bn::sprite_items::menu_options_arrow.create_sprite(52, 44, 1));

    for (int i = 0; i < _volumePoints.max_size(); i++) {
        _volumePoints.push_back(bn::sprite_items::menu_options_active_point.create_sprite(-60 + (16 * i), 44));
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
                // _updateBrightnessText();
                _updateBrightnessPoints();
            }
            else if (bn::keypad::left_held()) {
                bna::brightness_manager::set_brightness(brillo - CAMBIO_BRILLO);
                _updateBrightnessPoints();
                // _updateBrightnessText();
            }
        }
        else if (_idOpcion == 1) {
            constexpr bn::fixed CAMBIO_VOLUMEN = bn::fixed(0.01);
            if (bn::keypad::right_held()) {
                _volumen = bn::min(_volumen + CAMBIO_VOLUMEN, bn::fixed(1));
                bn::sound::set_master_volume((_volumen) * (_volumen));
                _updateVolumePoints();
            }
            else if (bn::keypad::left_held()) {
                bna::brightness_manager::set_brightness(brillo - CAMBIO_VOLUMEN);
                _volumen = bn::max(_volumen - CAMBIO_VOLUMEN, bn::fixed(0));
                bn::sound::set_master_volume((_volumen) * (_volumen));
                _updateVolumePoints();
            }
        }

        if (bn::keypad::b_pressed()) {
            _brillo = brillo;
            return bna::scene_type::MAIN_MENU;
        }

        _updateSelectedAnimation();
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
    int puntos_activos = (_volumen * 100 / 14).integer();
    for (int i = 0; i < _brightnessPoints.size(); i++) {
        if (i < puntos_activos) {
            _volumePoints[i].set_visible(true);
        }
        else {
            _volumePoints[i].set_visible(false);
        }
    }
}

void bna::OptionsMenu::_updateSelectedAnimation() {
    int offset = 1;
    if (_moveUp) {
        offset *= -1;
    }

    _brightnessArrows[0].set_y(_brightnessPoints[0].y());
    _brightnessArrows[1].set_y(_brightnessPoints[0].y());

    _volumeArrows[0].set_y(_volumePoints[0].y());
    _volumeArrows[1].set_y(_volumePoints[0].y());

    if (_idOpcion == 0) {
        _brightnessArrows[0].set_y(_brightnessPoints[0].y() + offset);
        _brightnessArrows[1].set_y(_brightnessPoints[0].y() + offset);
    }
    else {
        _volumeArrows[0].set_y(_volumePoints[0].y() + offset);
        _volumeArrows[1].set_y(_volumePoints[0].y() + offset);
    }

    if (_framesTranscurridos == 60) {
        _framesTranscurridos = 0;
        _moveUp = !_moveUp;
    }
    else {
        _framesTranscurridos++;
    }
}
