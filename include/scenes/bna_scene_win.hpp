#pragma once

#include "bn_vector.h"
#include "bn_array.h"

#include "bna_indicator.hpp"

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"
#include "bna_icon_roulette.hpp"

#include "bn_random.h"


namespace bna {
    namespace parts {
        enum class motors : int;
        enum class bodys : int;
        enum class wheels :int;
    }
    class SceneWin : public scene {
        public:
        SceneWin(bn::array<parts::motors, 3>& motores, bn::array<parts::bodys, 3>& cuerpos, bn::array<parts::wheels, 3>& ruedas);
        ~SceneWin() = default;

        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        bna::TextManager _texto_win;
        bna::TextManager _texto_continue;

        int _idOpcion;

        bn::vector<bna::Indicator, 6> _indicadoresOpciones;

        bn::array<parts::motors, 3>* _motores;
        bn::array<parts::bodys, 3>* _cuerpos;
        bn::array<parts::wheels, 3>* _ruedas;

        static inline bn::random _random;

        bool _aviableMotors();
        bool _aviableBodys();
        bool _aviableWheels();
        void _checkUnlockable();
    };
} // namespace bna
