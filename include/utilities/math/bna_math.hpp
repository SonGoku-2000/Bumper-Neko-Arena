#pragma once

#include "bn_fixed.h"
#include "bn_fixed_point.h"

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

        [[nodiscard]] constexpr bn::fixed_point interpolate_x(const bn::fixed_point& p1, const bn::fixed_point& p2, const bn::fixed x) {
            bn::fixed t = (x - p1.x()) / (p2.x() - p1.x());
            bn::fixed y = p1.y() + t * (p2.y() - p1.y());

            return bn::fixed_point(x, y);
        }

        [[nodiscard]] constexpr bn::fixed_point interpolate_y(const bn::fixed_point& p1, const bn::fixed_point& p2, const bn::fixed y) {
            bn::fixed t = (y - p1.y()) / (p2.y() - p1.y());
            bn::fixed x = p1.x() + t * (p2.x() - p1.x());

            return bn::fixed_point(x, y);
        }
    } // namespace math
} // namespace bna
