#pragma once

#include "bn_fixed.h"

namespace bna {
    namespace brightness_manager {
        constexpr bn::fixed MAX_BRIGHTNESS = 0.4;

        void set_brightness(bn::fixed brightness);
        bn::fixed get_brightness();

        void set_brightness_percent(int brightness);
        int get_brightness_percent();
    } // namespace brightness_manager

} // namespace bna
