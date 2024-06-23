#pragma once

#include "bn_optional.h"
#include "bn_fixed_point.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

namespace bna {
    union direction {
        struct {
            unsigned up : 1;
            unsigned down : 1;
            unsigned left : 1;
            unsigned right : 1;
        } keys;
        int data = 0;
    };


    [[nodiscard]] bn::optional<direction> read_keypad();

    void move_ninja(direction new_direction, direction& old_direction, bn::sprite_ptr& ninja_sprite,
        bn::sprite_animate_action<4>& ninja_animate_action);

    struct datos {
        bn::fixed_point pos1;
        bn::fixed angulo1;
        bn::fixed_point pos2;
        bn::fixed angulo2;
        bn::fixed_point pos3;
        bn::fixed angulo3;
        bn::fixed_point pos4;
        bn::fixed angulo4;
    };


    void link();
}

