#include "bna_preparing_connection.hpp"
#include "bn_core.h"

#include "bn_keypad.h"
#include "bna_link.hpp"

#include "bna_scene_type.hpp"


bna::PreparingConnection::PreparingConnection(int& id_propia) {
    _id_propia = &id_propia;
    _continuar = false;

    constexpr int ALINEACION_HORIZONTAL = -40;
    constexpr bool MOSTRAR_INDICADORES = false;

    _indicadores.push_back(bna::Indicator(bn::fixed_point(0, -50), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, -20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 0), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 20), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 40), MOSTRAR_INDICADORES));
    _indicadores.push_back(bna::Indicator(bn::fixed_point(ALINEACION_HORIZONTAL, 60), MOSTRAR_INDICADORES));

    _textoEstado = bna::TextManager(
        _indicadores[0].x(),
        _indicadores[0].y(),
        "Esperando nuevos jugadores"
    );
    _textoEstado.set_aligment(bn::sprite_text_generator::alignment_type::CENTER);
}


bn::optional<bna::scene_type> bna::PreparingConnection::update() {
    bool listo = false;

    bna::link::start mensajeEnviado;
    bna::link::start mensajeResivido;
    mensajeEnviado.keys.ready0 = listo;

    while (true) {
        mensajeEnviado.keys.ready0 = listo;
        if (bna::link::comprobarConexion(*_id_propia, mensajeEnviado, mensajeResivido)) {
            if (listo) {
                _textoEstado.updateText("Esperando respuesta ...");
            }
            else {
                _textoEstado.updateText("Start para empezar");
            }
            if (listo and mensajeResivido.keys.ready0) {
                break;
            }
            if (bn::keypad::start_held()) {
                listo = true;
            }
            if (bn::keypad::b_pressed()) {
                listo = false;
            }
        }
        else {
            _textoEstado.updateText("Esperando nuevos jugadores");
        }

        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}
