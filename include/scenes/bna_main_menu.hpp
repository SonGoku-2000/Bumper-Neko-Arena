#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"
#include "bna_indicator.hpp"

namespace bna {
    class MainMenu : public scene {
        public:
        MainMenu();
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        TextManager _textoTitulo;
        TextManager _textoJugar;
        TextManager _textoMultiplayer;
        TextManager _textoOpciones;
        TextManager _textoCreditos;

        enum class options :int {
            PLAY, MULTIPLAYER, OPTIONS, CREDITS
        };
        options _idOpcion;

        bn::vector<bna::Indicator, 5> _indicadores;
        bn::optional<bn::sprite_ptr> _puntero;
        bool _continuar;
    };
}
