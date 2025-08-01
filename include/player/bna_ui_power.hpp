#pragma once

#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"

namespace bna {
    class Player;
    class UIPower {
        public:
        UIPower();
        ~UIPower() = default;

        void update();
        void set_player(bna::Player& player);
        void set_camera(bn::camera_ptr& camera);
        void set_visible(bool visible);

        private:
        bn::sprite_ptr _power_icon;
        bna::Player* _player;

        void _set_sprite();
    };
} // namespace bna
