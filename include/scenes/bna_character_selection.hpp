#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"
#include "bna_indicator.hpp"

namespace bna {
    class CarBuilder;
    class CharacterSelection : public scene {
        public:
        CharacterSelection(CarBuilder& carBuilder);
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        TextManager _textoCharacterSeleccionado;
        TextManager _textoCharacter1;
        TextManager _textoCharacter2;
        TextManager _textoCharacter3;
        TextManager _textoCharacter4;
        TextManager _textoNext;
        TextManager _textoVolver;

        enum class opcionesCharacter :int {
            CAT_1,
            CAT_2,
            CAT_3,
            BIRD,
            NEXT,
            VOLVER
        };
        opcionesCharacter _idOpcion;

        bn::vector<bna::Indicator, 8> _indicadores;
        bn::optional<bn::sprite_ptr> _puntero;

        bool _continuar;
        bool _seleccionado;

        CarBuilder* _carBuilder;

        void _updateCharacterSelected();
    };
}
