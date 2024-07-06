#pragma once
#include "bn_display.h"

namespace bn::display {
    /**
     * @brief Returns the screen width in pixels (120).
     */
    [[nodiscard]] constexpr int half_width() {
        return hw::display::width()/2;
    }

    /**
     * @brief Returns the screen height in pixels (80).
     */
    [[nodiscard]] constexpr int half_height() {
        return hw::display::height()/2;
    }

    /**
     * @brief Returns the screen width in pixels (-120).
     */
    [[nodiscard]] constexpr int negative_half_width() {
        return hw::display::width()/2;
    }

    /**
     * @brief Returns the screen height in pixels (-80).
     */
    [[nodiscard]] constexpr int negative_half_height() {
        return hw::display::height()/2;
    }
}