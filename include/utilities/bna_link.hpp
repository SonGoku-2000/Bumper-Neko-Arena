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

        bool comprobarConexion(int& idConeccion, const bna::link::start& mensajeEnviado, bna::link::start& mensajeRecibido);

        bool checkJugadoresReady(const bna::link::start& mensajeEnviado, bna::link::start& mensajeRecibido);

        bn::vector<CarBuilder, 3> getCarBuilders(const CarBuilder self_car_builder);
        bn::array<bn::fixed_point, 3> getCarEjes(const bn::fixed_point eje_enviado);

        void reset();
    } // namespace link
}
