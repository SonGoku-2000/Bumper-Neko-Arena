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

bn::array<bn::optional<bna::link::fixed>, 4> bna::link::get_fixed(const bn::fixed number, const int id) {
    constexpr int PRESITION = 4;
    bna::link::fixed mensaje_enviar;
    mensaje_enviar.fixed.data = bn::fixed_t<PRESITION>(number).data();
    mensaje_enviar.fixed.id = id;
    bn::link::send(mensaje_enviar.data);

    bn::array<bn::optional<bna::link::fixed>, 4> respuesta;
    bna::link::fixed mensaje_recibido;

    constexpr int max_failed_retries = 5;
    int failed_retries = 0;
    while (failed_retries <= max_failed_retries) {
        if (bn::optional<bn::link_state> link_state = bn::link::receive()) {
            for (int i = 0; i < link_state->other_players().size(); i++) {
                const bn::link_player& other_player = link_state->other_players()[i];
                mensaje_recibido.data = other_player.data();
                respuesta[other_player.id()] = mensaje_recibido;
            }
        }
        else {
            ++failed_retries;
        }
    }
    return respuesta;
}

bn::array<bn::optional<bn::fixed_point>, 4> bna::link::get_fixed_point(const bn::fixed_point number, const int id) {
    bn::array<bn::optional<bn::fixed_point>, 4> respuesta;
    (void)number;
    (void)id;

    // bn::array<bn::optional<bn::fixed>, 4> respuesta_x = bna::link::get_fixed(number.x(), id);
    // bn::array<bn::optional<bn::fixed>, 4> respuesta_y = bna::link::get_fixed(number.y(), id);

    for (int i = 0; i < respuesta.size(); i++) {
        // if (respuesta_x[i].has_value()) {
        // if (respuesta_x[i].has_value() && respuesta_y[i].has_value()) {
            // respuesta[i] = bn::fixed_point(respuesta_x[i].value(), respuesta_y[i].value());
    // }
    }
    return respuesta;
}


void bna::link::getCarEjes(const bn::fixed_point eje_enviado, bn::array<bn::fixed_point, 4>& ejes) {
    int failed_retries = 0;

    bna::link::reset();
    bna::link::eje_presionado mensaje_enviar;
    mensaje_enviar.eje.x = eje_enviado.x().integer();
    mensaje_enviar.eje.y = eje_enviado.y().integer();

    bn::fixed_point eje_recibido;
    bna::link::eje_presionado mensajeRecibido;

    while (true) {
        bn::link::send(mensaje_enviar.data);
        if (bn::optional<bn::link_state> link_state = bn::link::receive()) {
            if (link_state->player_count() == link_state->other_players().size() + 1) {
                for (int i = 0; i < link_state->other_players().size(); i++) {
                    const bn::link_player& other_player = link_state->other_players()[i];
                    mensajeRecibido.data = other_player.data();

                    eje_recibido = bn::fixed_point(mensajeRecibido.eje.x, mensajeRecibido.eje.y);
                    ejes[other_player.id()] = eje_recibido;
                }
                // return ejes;
                ejes[link_state->current_player_id()] = eje_enviado;
                return;
            }
        }
        else {
            ++failed_retries;
        }
    }
    // return ejes;
}

int bna::link::sinc(const int id) {
    bn::link::send(id);
    int respuesta = id;

    constexpr int max_failed_retries = 5;
    int failed_retries = 0;

    while (failed_retries <= max_failed_retries) {
        if (bn::optional<bn::link_state> link_state = bn::link::receive()) {
            if (link_state->current_player_id() == 0) {
                respuesta = id;
                break;
            }
            else {
                for (int i = 0; i < link_state->other_players().size(); i++) {
                    if (link_state->other_players()[i].id() == 0) {
                        respuesta = link_state->other_players()[i].data();
                        break;
                    }
                }
            }
        }
        else {
            ++failed_retries;
        }
    }
    return respuesta;
}
void bna::link::reset() {
    bn::link::deactivate();
    bn::link::send(0);
    constexpr int max_failed_retries = 5;
    int failed_retries = 0;

    while (failed_retries <= max_failed_retries) {
        if (bn::optional<bn::link_state> link_state = bn::link::receive()) {

        }
        else {
            ++failed_retries;
        }
    }
}

