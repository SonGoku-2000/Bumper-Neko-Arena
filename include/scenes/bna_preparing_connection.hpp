#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"
#include "bna_indicator.hpp"

namespace bna {
    class PreparingConnection : public scene {
        public:
        PreparingConnection(int& id_propia);
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        TextManager _textoEstado;

        bn::vector<bna::Indicator, 6> _indicadores;
        bool _continuar;
        int* _id_propia;
    };
}
