#pragma once

#include "bn_optional.h"
#include "bn_fixed_point.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"
#include "bn_array.h"

namespace bna {
    namespace parts {
        enum class motors : int;
        enum class bodys : int;
        enum class wheels :int;
    } // namespace name
    class CarBuilder;
    namespace link {
        union start {
            struct {
                bool ready0;
            } keys;
            int data = 0;
        };

        union car_builder {
            struct {
                parts::motors motor : 2;
                parts::bodys body : 2;
                parts::wheels wheel : 2;
            } car_parts;
            int data = 0;
        };

        union eje_presionado {
            struct {
                int x : 2;
                int y : 2;
            }eje;
            int data = 0;
        };

        union fixed {
            struct {
                unsigned id : 3;
                unsigned data : 13;
            }fixed;
            int data;
        };
        union speed_info {
            struct {
                unsigned id : 4;
                int speed : 4;
                int external_force_x: 4;
                int external_force_y: 4;
            }info;
            int data;
        };
        union link_datas{
            fixed link_fixed;
            speed_info link_speed_info;
        };

        bn::array<bn::optional<bna::link::fixed>,4> get_fixed(const bn::fixed number,const int id);
        bn::array<bn::optional<bna::link::speed_info>,4> get_speed_info(const bn::fixed speed,const bn::fixed_point external_force,const int id);

        bool comprobarConexion(int& idConeccion, const bna::link::start& mensajeEnviado, bna::link::start& mensajeRecibido);

        bool checkJugadoresReady(const bna::link::start& mensajeEnviado, bna::link::start& mensajeRecibido);

        bn::vector<CarBuilder, 3> getCarBuilders(const CarBuilder self_car_builder);
        void getCarEjes(const bn::fixed_point eje_enviado, bn::array<bn::fixed_point, 4>& ejes);

        int sinc(const int id);
        void reset();
    } // namespace link
}
