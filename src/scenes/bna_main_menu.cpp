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
    _idOpcion = options::PLAY;

    constexpr int ALINEACION_HORIZONTAL = -40;
    constexpr bool MOSTRAR_INDICADORES = false;
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 0), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 60), MOSTRAR_INDICADORES));

    constexpr int OFFSET_HORIZONTAL_TEXTO = 10;
    _textoJugar = bna::TextManager(
        _indicadores[int(options::PLAY)].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[int(options::PLAY)].y(),
        "Play"
    );

    _textoMultiplayer = bna::TextManager(
        _indicadores[int(options::MULTIPLAYER)].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[int(options::MULTIPLAYER)].y(),
        "Multiplayer"
    );

    _textoOpciones = bna::TextManager(
        _indicadores[int(options::OPTIONS)].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[int(options::OPTIONS)].y(),
        "Options"
    );

    _textoCreditos = bna::TextManager(
        _indicadores[int(options::CREDITS)].x() + OFFSET_HORIZONTAL_TEXTO,
        _indicadores[int(options::CREDITS)].y(),
        "Credits"
    );

    _puntero = bn::sprite_items::pointer.create_sprite(_indicadores[int(_idOpcion)]);
}


bn::optional<bna::scene_type> bna::MainMenu::update() {
    while (!_continuar) {
        if (bn::keypad::down_pressed()) {
            _idOpcion = options(bna::loop(int(_idOpcion) + 1, 0, int(options::CREDITS)));
            _puntero->set_position(_indicadores[int(_idOpcion)]);
        }
        else if (bn::keypad::up_pressed()) {
            _idOpcion = options(bna::loop(int(_idOpcion) - 1, 0, int(options::CREDITS)));
           _puntero->set_position(_indicadores[int(_idOpcion)]);
        }

        if (bn::keypad::a_pressed()) {
            if (_idOpcion == options::PLAY) {
                return bna::scene_type::CAR_SELECTION;
            }
            else if(_idOpcion== options::MULTIPLAYER){
                return bna::scene_type::PREPARING_CONNECTION;
            }
            else if (_idOpcion == options::OPTIONS) {
                return bna::scene_type::OPTION_MENU;
            }
        }

        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

