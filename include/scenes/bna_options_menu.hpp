#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"
#include "bna_indicator.hpp"

#include "bn_array.h"

#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"


namespace bna {
    class OptionsMenu : public scene {
        public:
        OptionsMenu(bn::fixed& brillo_memory, bn::fixed& volume);
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        TextManager _textoBrillo;
        TextManager _textoVolver;

        bn::fixed& _brillo;
        bn::fixed& _volumen;
        int _idOpcion;

        bn::regular_bg_ptr _background;

        bn::vector<bna::Indicator, 4> _indicadores;

        bn::vector<bn::sprite_ptr, 7> _brightnessPoints;
        bn::vector<bn::sprite_ptr, 2> _brightnessArrows;

        bn::vector<bn::sprite_ptr, 7> _volumePoints;
        bn::vector<bn::sprite_ptr, 2> _volumeArrows;

        bn::optional<bn::sprite_ptr> _puntero;
        bool _continuar;
        bool _moveUp;
        int _framesTranscurridos;

        void _updateBrightnessText();
        void _updateBrightnessPoints();
        void _updateVolumePoints();
        void _updateSelectedAnimation();
    };
}