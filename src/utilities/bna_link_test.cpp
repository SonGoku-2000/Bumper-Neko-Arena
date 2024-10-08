#include "bn_core.h"
#include "bn_link.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_link_state.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bna_link_test.hpp"
#include "bn_log.h"
namespace bna {


    [[nodiscard]] bn::optional<direction> read_keypad() {
        bn::optional<direction> result;

        if (bn::keypad::up_held()) {
            if (!result) {
                result = direction();
            }

            result->keys.up = true;
        }

        if (bn::keypad::down_held()) {
            if (!result) {
                result = direction();
            }

            result->keys.down = true;
        }

        if (bn::keypad::left_held()) {
            if (!result) {
                result = direction();
            }

            result->keys.left = true;
        }

        if (bn::keypad::right_held()) {
            if (!result) {
                result = direction();
            }

            result->keys.right = true;
        }

        return result;
    }

    void move_ninja(direction new_direction, direction& old_direction, bn::sprite_ptr& ninja_sprite,
        bn::sprite_animate_action<4>& ninja_animate_action) {
        (void)ninja_animate_action;
        bool direction_changed = false;

        if (new_direction.keys.left) {
            ninja_sprite.set_x(ninja_sprite.x() - 1);

            if (new_direction.data != old_direction.data) {
                direction_changed = true;
                // ninja_animate_action = bn::create_sprite_animate_action_forever(
                //             ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 8, 9, 10, 11);
            }
        }
        else if (new_direction.keys.right) {
            ninja_sprite.set_x(ninja_sprite.x() + 1);

            if (new_direction.data != old_direction.data) {
                direction_changed = true;
                // ninja_animate_action = bn::create_sprite_animate_action_forever(
                //             ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 12, 13, 14, 15);
            }
        }

        if (new_direction.keys.up) {
            ninja_sprite.set_y(ninja_sprite.y() - 1);

            if (new_direction.data != old_direction.data) {
                direction_changed = true;
                // ninja_animate_action = bn::create_sprite_animate_action_forever(
                //             ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 4, 5, 6, 7);
            }
        }
        else if (new_direction.keys.down) {
            ninja_sprite.set_y(ninja_sprite.y() + 1);

            if (new_direction.data != old_direction.data) {
                direction_changed = true;
                // ninja_animate_action = bn::create_sprite_animate_action_forever(
                //             ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);
            }
        }

        if (direction_changed) {
            old_direction = new_direction;
        }
    }
}


bool comprobarConeccion(int& idConeccion, const bna::start& mensajeEnviado, bna::start& mensajeRecibido) {
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

void esperarJugadores(int& id_propia) {
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::vector<bn::sprite_ptr, 64> sprites_mensaje_espera;
    text_generator.set_alignment(bn::sprite_text_generator::alignment_type::CENTER);
    bool listo = false;

    bna::start mensajeEnviado;
    bna::start mensajeResivido;
    mensajeEnviado.keys.ready0 = listo;

    while (true) {
        mensajeEnviado.keys.ready0 = listo;
        if (comprobarConeccion(id_propia, mensajeEnviado, mensajeResivido)) {
            sprites_mensaje_espera.clear();

            if (listo) {
                text_generator.generate(0, 44, "esperando respuesta ...", sprites_mensaje_espera);
            }
            else {
                text_generator.generate(0, 44, "start para empezar", sprites_mensaje_espera);
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
            sprites_mensaje_espera.clear();
            text_generator.generate(0, 44, "Esperando nuevos jugadores", sprites_mensaje_espera);
        }

        bn::core::update();
    }
}


void bna::testLimitValues() {
    constexpr int  PRECICION = 1;
    // Precision  1 va de pasos de 0.5     max 8190
    // Precision  2 va de pasos de 0.25    max 4095
    // Precision  3 va de pasos de 0.125   max 2047.5
    // Precision  4 va de pasos de 0.0625  max 1023.75
    // Precision  5 va de pasos de 0,03125 max 511.875
    // Precision  6 va de pasos de 0,01562 max 255,937
    // Precision  7 va de pasos de 0,00781 max 127,968
    // Precision  8 va de pasos de 0,00390 max 63,9843
    // Precision  9 va de pasos de 0,00195 max 31,9921
    // Precision 10 va de pasos de 0,00097 max 15,9960
    // Precision 11 va de pasos de 0,00048 max 7,99804
    // Precision 12 va de pasos de 0,00024 max 1,99951
    // Precision 13 va de pasos de 0,00012 max 3,99902
    BN_LOG("Position limite 14: ", bn::fixed_t<PRECICION>().from_data(0b11111111111101));
    BN_LOG("Position limite 13: ", bn::fixed_t<PRECICION>().from_data(0b1111111111101));
    BN_LOG("Position limite 12: ", bn::fixed_t<PRECICION>().from_data(0b111111111101));
    BN_LOG("Position limite 11: ", bn::fixed_t<PRECICION>().from_data(0b11111111101));
    BN_LOG("Position limite 10: ", bn::fixed_t<PRECICION>().from_data(0b1111111101));
    BN_LOG("Position limite 9: ", bn::fixed_t<PRECICION>().from_data(0b111111101));
    BN_LOG("Position limite 8: ", bn::fixed_t<PRECICION>().from_data(0b11111101));
    BN_LOG("Position limite 7: ", bn::fixed_t<PRECICION>().from_data(0b1111101));
    BN_LOG("Position limite 6: ", bn::fixed_t<PRECICION>().from_data(0b111101));
    BN_LOG("Position limite 5: ", bn::fixed_t<PRECICION>().from_data(0b11101));
    BN_LOG("Position limite 4: ", bn::fixed_t<PRECICION>().from_data(0b1101));
    BN_LOG("Position limite 3: ", bn::fixed_t<PRECICION>().from_data(0b101));
    BN_LOG("Position limite 2: ", bn::fixed_t<PRECICION>().from_data(0b01));
    BN_LOG("Pases: ", bn::fixed_t<PRECICION>().from_data(1));
}

void bna::link_test() {

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    bn::string_view info_text_lines[] = {
        "PAD: move other player's ninja",
    };

    common::info info("Link communication", info_text_lines, text_generator);
    bn::vector<bn::sprite_ptr, 64> messages_per_second_sprites;


    // int id_propia;

    // esperarJugadores(id_propia);
    constexpr int  PRECICION = 2;
    bn::fixed  pos;

    int frames_counter = 0;
    int messages_counter = 0;

    bna::mensaje mensaje;
    bna::mensaje nuevoMensaje;

    while (true) {
        if (bn::keypad::a_held()) {
            nuevoMensaje.informacion.id = 2;
            pos += bn::fixed(0.00001);
            bn::fixed_t<PRECICION> pos_enviar = pos;
            nuevoMensaje.informacion.pos = pos_enviar.data();
            mensaje = nuevoMensaje;
            bn::link::send(mensaje.data);
            BN_LOG(pos);
        }

        if (bn::keypad::b_held()) {
            nuevoMensaje.informacion.id = 1;
            pos -= bn::fixed(0.001);
            bn::fixed_t<PRECICION> pos_enviar = pos;
            nuevoMensaje.informacion.pos = pos_enviar.data();
            mensaje = nuevoMensaje;
            bn::link::send(mensaje.data);
            BN_LOG(pos);
        }

        // if (mensaje.informacion.pos1 != nuevoMensaje.informacion.pos1 or
        //     mensaje.informacion.angulo1 != nuevoMensaje.informacion.angulo1
        //     ) {
        //     mensaje = nuevoMensaje;
        //     bn::link::send(mensaje.data);
        // }

        int max_failed_retries = 5;
        int failed_retries = 0;

        while (failed_retries <= max_failed_retries) {
            if (bn::optional<bn::link_state> link_state = bn::link::receive()) {
                BN_LOG("------");
                const bn::link_player& first_other_player = link_state->other_players().front();
                bna::mensaje new_mensaje;
                new_mensaje.data = first_other_player.data();
                BN_LOG("ID ", new_mensaje.informacion.id);
                BN_LOG("Position ", new_mensaje.informacion.pos);
                BN_LOG("Position ", bn::fixed_t<PRECICION>().from_data(new_mensaje.informacion.pos));
                failed_retries = 0;
                ++messages_counter;
            }
            else {
                ++failed_retries;
            }
        }

        if (++frames_counter == 60) {
            bn::string<64> messages_per_second = "Messages per second: ";
            messages_per_second += bn::to_string<18>(messages_counter);
            messages_per_second_sprites.clear();
            text_generator.generate(0, 44, messages_per_second, messages_per_second_sprites);
            frames_counter = 0;
            messages_counter = 0;
        }

        // ninja_animate_action.update();
        info.update();
        bn::core::update();
    }
}

