#include "bna_options_menu.hpp"
#include "bn_core.h"

#include "bna_brightness_manager.hpp"
#include "bn_sprite_items_pointer.h"

#include "bna_scene_type.hpp"

#include "bn_keypad.h"
#include "bna_loop_value.hpp"

bna::OptionsMenu::OptionsMenu() {
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
        "Brightness"
    );

    _textoVolver = bna::TextManager(
        _indicadores[1].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[1].y(),
        "Back"
    );

    _puntero = bn::sprite_items::pointer.create_sprite(_indicadores[_idOpcion]);
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
            }
            else if (bn::keypad::left_held()) {
                bna::brightness_manager::set_brightness(brillo - CAMBIO_BRILLO);
            }
        }


        if (bn::keypad::a_pressed()) {
            if (_idOpcion == 1) {
                return bna::scene_type::MAIN_MENU;
            }
        }
        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

