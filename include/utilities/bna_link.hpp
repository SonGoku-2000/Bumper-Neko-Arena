#pragma once

#include "bn_optional.h"
#include "bn_fixed_point.h"
#include "bn_sprite_ptr.h"

namespace bna {
    namespace link {
        union start {
            struct {
                bool ready0;
            } keys;
            int data = 0;
        };

        bool comprobarConexion(int& idConeccion, const bna::link::start& mensajeEnviado, bna::link::start& mensajeRecibido);

        bool checkJugadoresReady(const bna::link::start& mensajeEnviado, bna::link::start& mensajeRecibido);
    } // namespace link
}
