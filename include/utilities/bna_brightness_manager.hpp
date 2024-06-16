#pragma once

#include "bn_fixed.h"

namespace bna {
    namespace brightness_manager {
        void set_brightness(bn::fixed brightness);
        bn::fixed get_brightness();
    } // namespace brightness_manager

} // namespace bna
