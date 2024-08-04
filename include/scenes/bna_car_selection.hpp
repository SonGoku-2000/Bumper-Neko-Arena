#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"
#include "bna_indicator.hpp"

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bna_icon_roulette.hpp"

namespace bna {
    namespace parts {
        enum class motors : int;
        enum class bodys : int;
        enum class wheels :int;
    } // namespace name
    class CarBuilder;
    class CarSelection : public scene {
        public:
        CarSelection(CarBuilder& carBuilder,
            bn::array<parts::motors, 3>& motores, bn::array<parts::bodys, 3>& cuerpos, bn::array<parts::wheels, 3>& ruedas
        );
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        TextManager _textoStats;
        TextManager _textoCuerpo;
        TextManager _textoMotor;
        TextManager _textoRueda;
        TextManager _textoPlay;
        TextManager _textoVolver;

        enum class opcionesPartes :int {
            CUERPO,
            MOTOR,
            WHEEL,
            PLAY,
            VOLVER
        };
        opcionesPartes _idOpcion;

        bn::vector<bna::Indicator, 6> _indicadores;
        bn::optional<bn::sprite_ptr> _puntero;

        bna::IconRoulette _bodysRoulette;
        bna::IconRoulette _motorsRoulette;
        bna::IconRoulette _wheelsRoulette;

        bna::parts::motors _idMotor;
        bna::parts::bodys _idBody;
        bna::parts::wheels _idWheel;

        bn::array<parts::motors, 3> _motores;
        bn::array<parts::bodys, 3> _cuerpos;
        bn::array<parts::wheels, 3> _ruedas;

        bool _continuar;

        CarBuilder* _carBuilder;

        void _updateStatsText();
        void _updateBodyText();
        void _updateMotorText();
        void _updateWheelText();

        bool _checkValidCombination();

        void _generateMotorRoulette();
        void _generateBodyRoulette();
        void _generateWheelRoulette();
    };
}
