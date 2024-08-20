#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"
#include "bna_indicator.hpp"

#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_animate_actions.h"


namespace bna {
    enum class CharactersId :int;
    class CharacterSelection : public scene {
        public:
        CharacterSelection(CharactersId& character);
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        // TextManager _textoCharacterSeleccionado;
        // TextManager _textoCharacter1;
        // TextManager _textoCharacter2;
        // TextManager _textoCharacter3;
        // TextManager _textoCharacter4;
        // TextManager _textoNext;
        // TextManager _textoVolver;

        enum class opcionesCharacter :int {
            BLACK,
            PERSIAN,
            SIAMESE,
            TRICOLOUR,
            BACK
        };
        opcionesCharacter _idOpcion;
        opcionesCharacter _idOpcionSeleccionada;
        CharactersId* _character;

        bool _continuar;
        bool _seleccionado;

        bn::optional<bn::regular_bg_ptr> _background;

        bn::vector<bna::Indicator, 8> _indicadores;
        bn::vector<bn::sprite_ptr, 4> _iconosCharacters;
        bn::optional<bn::sprite_animate_action<12>> _animationIcon;
        bn::optional<bn::sprite_ptr> _puntero;
        bn::sprite_ptr _backButton;

        bn::optional<bn::sprite_ptr> _character_image;
        bn::optional<bn::sprite_animate_action<4>> _pointerAnimation;

        void _updateArrowPress();
        void _updateCharacterSelected();
        void _updateCharacterPointer();
        void _animateSelectedIcon();
    };
}
