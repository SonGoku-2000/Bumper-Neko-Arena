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

    union start {
        struct {
            bool ready0;
            bool ready1;
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
        unsigned id : 2;
        unsigned pos:14;
    };
    union mensaje {
        datos informacion;
        int data = 0;
        mensaje() {}
        ~mensaje() {}
    };

    void link_test();
    void testLimitValues();
}

