#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"
#include "bna_indicator.hpp"

namespace bna {
    namespace parts {
        enum class motors : int;
        enum class bodys : int;
        enum class wheels :int;
    } // namespace name
    class CarBuilder;
    class CarSelection : public scene {
        public:
        CarSelection(CarBuilder& carBuilder);
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

        bna::parts::motors _idMotor;
        bna::parts::bodys _idBody;
        bna::parts::wheels _idWheel;

        bool _continuar;

        CarBuilder* _carBuilder;

        void _updateStatsText();
        void _updateBodyText();
        void _updateMotorText();
        void _updateWheelText();
    };
}
