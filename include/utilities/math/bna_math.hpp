#pragma once

#include "bn_fixed.h"

namespace bna {
    namespace math {
        [[nodiscard]] constexpr bn::fixed modulo_positivo(bn::fixed num, int divisor) {
            int n = num.division(divisor).integer();
            num = num - (n * divisor);
            if (num < 0) {
                return num + divisor;
            }
            return num;
        }
    } // namespace math
} // namespace bna
