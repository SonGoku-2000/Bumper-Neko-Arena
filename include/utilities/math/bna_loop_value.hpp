#pragma once

namespace bna {
    constexpr int loop(int value, int min, int max) {
        if (value < min) {
            return max;
        }
        if (value > max) {
            return min;
        }
        return value;
    }
} // namespace bna
