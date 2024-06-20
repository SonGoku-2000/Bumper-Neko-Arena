#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"
#include "bna_indicator.hpp"

namespace bna {
    namespace parts {
        enum class MOTORS : int;
        enum class BODYS : int;
        enum class WHEELS :int;
    } // namespace name
    class CarSelection : public scene {
        public:
        CarSelection();
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        TextManager _textoStats;
        TextManager _textoCuerpo;
        TextManager _textoMotor;
        TextManager _textoRueda;
        TextManager _textoVolver;

        enum class opcionesPartes :int {
            CUERPO,
            MOTOR,
            WHEEL,
            VOLVER
        };
        opcionesPartes _idOpcion;

        bn::vector<bna::Indicator, 5> _indicadores;
        bn::optional<bn::sprite_ptr> _puntero;

        bna::parts::MOTORS _idMotor;
        bna::parts::BODYS _idBody;
        bna::parts::WHEELS _idWheel;

        bool _continuar;

        void _updateStatsText();
        void _updateBodyText();
        void _updateMotorText();
        void _updateWheelText();
    };
}
