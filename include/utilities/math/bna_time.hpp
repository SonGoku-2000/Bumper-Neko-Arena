#pragma once

#include "bn_fixed.h"

namespace bna {
    namespace time {
        constexpr int seconds_to_frames(const bn::fixed seconds) {
            return seconds.multiplication(60).integer();
        }

        constexpr bn::fixed frames_to_seconds(const int frames) {
            return bn::fixed(frames).division(60);
        }
    } // namespace time
} // namespace bna