#pragma once

#include "bn_camera_ptr.h"

#include "bna_ui_life.hpp"
#include "bna_ui_power.hpp"

namespace bna {
    class Player;
    class UI {
        public:
        UI();
        ~UI() = default;
        void update();
        void set_player(Player& player);

        private:
        bn::camera_ptr _camera;
        UILife _ui_life;
        UIPower _ui_power;
        Player* _player;
        bn::fixed _previous_life;

        enum state {
            STOP,
            SHAKE,
            MOVE_RIGHT,
            MOVE_LEFT
        };
        state _state;
        enum phase {
            STARTING, READY,
        };
        phase _phase;
    };
} // namespace bna
