#include "bna_link.hpp"

#include "bn_link.h"
#include "bn_link_state.h"

bool bna::link::comprobarConexion(int& idConeccion, const bna::link::start& mensajeEnviado, bna::link::start& mensajeRecibido) {
    constexpr int max_failed_retries = 5;
    int failed_retries = 0;

    bn::link::send(mensajeEnviado.data);
    while (failed_retries <= max_failed_retries) {
        if (bn::optional<bn::link_state> link_state = bn::link::receive()) {
            idConeccion = link_state->current_player_id();
            const bn::link_player& first_other_player = link_state->other_players().front();
            mensajeRecibido.data = first_other_player.data();
            return true;
        }
        else {
            ++failed_retries;
        }
    }
    return false;
}

bool bna::link::checkJugadoresReady(const bna::link::start& mensajeEnviado, bna::link::start& mensajeRecibido) {
    constexpr int max_failed_retries = 5;
    int failed_retries = 0;

    bn::link::send(mensajeEnviado.data);
    while (failed_retries <= max_failed_retries) {
        if (bn::optional<bn::link_state> link_state = bn::link::receive()) {
            for (int i = 0; i < link_state->other_players().size(); i++) {
                const bn::link_player& first_other_player = link_state->other_players()[i];
                mensajeRecibido.data = first_other_player.data();
                if (!mensajeRecibido.keys.ready0) {
                    return false;
                }

            }

            return true;
        }
        else {
            ++failed_retries;
        }
    }
    return false;
}
