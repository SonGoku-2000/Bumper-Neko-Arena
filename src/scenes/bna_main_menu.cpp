#include "bna_main_menu.hpp"
#include "bn_core.h"

#include "bna_scene_type.hpp"

#include "bn_sprite_items_pointer.h"

#include "bn_keypad.h"

#include "bna_loop_value.hpp"

bna::MainMenu::MainMenu() {
    _textoTitulo = bna::TextManager(0, -60, "Bumper Neko Arena");
    _textoTitulo.set_aligment(bn::sprite_text_generator::alignment_type::CENTER);
    _continuar = false;
    _idOpcion = 0;

    constexpr int ALINEACION_HORIZONTAL = -40;
    constexpr bool MOSTRAR_INDICADORES = false;
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 0), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 40), MOSTRAR_INDICADORES));

    constexpr int OFFSET_HORIZONTAL_TEXTO = 10;
    _textoJugar = bna::TextManager(
        _indicadores[0].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[0].y(),
        "Play"
    );

    _textoOpciones = bna::TextManager(
        _indicadores[1].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[1].y(),
        "Options"
    );

    _textoCreditos = bna::TextManager(
        _indicadores[2].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[2].y(),
        "Credits"
    );

    _puntero = bn::sprite_items::pointer.create_sprite(_indicadores[_idOpcion]);
}


bn::optional<bna::scene_type> bna::MainMenu::update() {
    while (!_continuar) {
        if (bn::keypad::down_pressed()) {
            _idOpcion = bna::loop(_idOpcion + 1, 0, _indicadores.size() - 1);
            _puntero->set_position(_indicadores[_idOpcion]);
        }
        else if (bn::keypad::up_pressed()) {
            _idOpcion = bna::loop(_idOpcion - 1, 0, _indicadores.size() - 1);
            _puntero->set_position(_indicadores[_idOpcion]);
        }
        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

