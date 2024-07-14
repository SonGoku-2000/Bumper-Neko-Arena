#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"
#include "bna_indicator.hpp"

#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"


namespace bna {
    enum class Characters :int;
    class CharacterSelection : public scene {
        public:
        CharacterSelection(Characters& character);
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
        opcionesCharacter _idOpcionSeleccionada;
        Characters* _character;

        bool _continuar;
        bool _seleccionado;


        bn::vector<bna::Indicator, 8> _indicadores;
        bn::vector<bn::sprite_ptr, 4> _iconosCharacters;
        bn::optional<bn::sprite_ptr> _puntero;

        bn::optional<bn::regular_bg_ptr > _character_image;

        void _updateArrowPress();
        void _updateCharacterSelected();
        void _updateCharacterPointer();
    };
}
