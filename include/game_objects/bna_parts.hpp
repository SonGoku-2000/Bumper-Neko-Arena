#pragma once

#include "bn_fixed.h"


namespace bna {
    struct Stats {
        bn::fixed maxSpeed = 0;
        bn::fixed aceleration = 0;
        bn::fixed turn = 0;
        bn::fixed weight = 0;
    };

    namespace parts {
        enum class MOTORS : int {
            SLOW,
            MEDIUM,
            FAST,
            MAX
        };

        enum class BODYS : int {
            LIGHT,
            MEDIUM,
            HEAVY,
            MAX
        };

        enum class WHEELS :int {
            NORMAL,
            SPIKES,
            MAX
        };

        Stats getMotor(MOTORS id);
        Stats getBody(BODYS id);
        Stats getWheels(WHEELS id);
    } // namespace name
} // namespace bna
