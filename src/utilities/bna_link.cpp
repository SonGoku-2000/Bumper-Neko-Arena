#include "bna_link.hpp"

#include "bn_link.h"
#include "bn_link_state.h"
#include "bna_car_builder.hpp"

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

bn::vector<bna::CarBuilder, 3> bna::link::getCarBuilders(const bna::CarBuilder self_car_builder) {
    constexpr int max_failed_retries = 5;
    int failed_retries = 0;

    bna::link::car_builder mensaje;
    mensaje.car_parts.body = self_car_builder.body;
    mensaje.car_parts.motor = self_car_builder.motor;
    mensaje.car_parts.wheel = self_car_builder.wheel;
    bn::link::send(mensaje.data);

    bn::vector<bna::CarBuilder, 3> carros;
    bna::CarBuilder car_builder;
    bna::link::car_builder mensajeRecibido;

    while (failed_retries <= max_failed_retries) {
        if (bn::optional<bn::link_state> link_state = bn::link::receive()) {
            for (int i = 0; i < link_state->other_players().size(); i++) {
                const bn::link_player& other_player = link_state->other_players()[i];
                mensajeRecibido.data = other_player.data();
                car_builder.body = mensajeRecibido.car_parts.body;
                car_builder.motor = mensajeRecibido.car_parts.motor;
                car_builder.wheel = mensajeRecibido.car_parts.wheel;
                if (i < carros.size()) {
                    carros[i] = car_builder;
                }
                else {
                    carros.push_back(car_builder);
                }
            }
        }
        else {
            ++failed_retries;
        }
    }
    return carros;
}
