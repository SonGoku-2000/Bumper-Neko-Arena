#pragma once

#include "bn_fixed.h"


namespace bna {
    struct Stats {
        bn::fixed maxSpeed = 0;
        bn::fixed aceleration = 0;
        bn::fixed turn = 0;
        bn::fixed weight = 0;

        Stats() {}
        Stats(bn::fixed newMaxSpeed, bn::fixed newAceleration, bn::fixed newTurn, bn::fixed newWeight) {
            maxSpeed = newMaxSpeed;
            aceleration = newAceleration;
            turn = newTurn;
            weight = newWeight;
        }

        Stats operator+(const Stats& other) const {
            return Stats(
                maxSpeed + other.maxSpeed,
                aceleration + other.aceleration,
                turn + other.turn,
                weight + other.weight
            );
        }
    };

    namespace parts {
        enum class motors : int {
            SLOW,
            MEDIUM,
            FAST,
            MAX
        };

        enum class bodys : int {
            LIGHT,
            MEDIUM,
            HEAVY,
            MAX
        };

        enum class wheels :int {
            NORMAL,
            SPIKES,
            MAX
        };

        Stats getMotor(motors id);
        Stats getBody(bodys id);
        Stats getWheels(wheels id);
    } // namespace name
} // namespace bna
