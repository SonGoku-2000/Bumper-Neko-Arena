#pragma once

#include "bn_optional.h"
#include "bn_fixed_point.h"
#include "bn_sprite_ptr.h"

namespace bna {
    union start {
        struct {
            bool ready0;
        } keys;
        int data = 0;
    };

    bool comprobarConexion(int& idConeccion, const bna::start& mensajeEnviado, bna::start& mensajeRecibido);
}
