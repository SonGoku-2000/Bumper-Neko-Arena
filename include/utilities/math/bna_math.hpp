#pragma once

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_math.h"

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


        [[nodiscard]] constexpr bn::fixed lerp(bn::fixed number_1, bn::fixed number_2, bn::fixed t) {
            return number_1 + t * (number_2 - number_1);
        }

        [[nodiscard]] constexpr bn::fixed_point lerp(const bn::fixed_point& point_1, const bn::fixed_point& point_2, const bn::fixed t) {
            return bn::fixed_point(point_1.x() + t * (point_2.x() - point_1.x()), point_1.y() + t * (point_2.y() - point_1.y()));
        }


        [[nodiscard]] constexpr bn::fixed_point interpolate_x(const bn::fixed_point& p1, const bn::fixed_point& p2, const bn::fixed x) {
            constexpr bn::fixed EPSILON(0.1);
            if (bn::abs(p2.x() - p1.x()) < EPSILON) {
                return  bn::fixed_point(x, p2.y());
            }
            bn::fixed t = (x - p1.x()) / (p2.x() - p1.x());
            bn::fixed y = p1.y() + t * (p2.y() - p1.y());

            return bn::fixed_point(x, y);
        }

        [[nodiscard]] constexpr bn::fixed_point interpolate_y(const bn::fixed_point& p1, const bn::fixed_point& p2, const bn::fixed y) {
            constexpr bn::fixed EPSILON(0.1);
            if (bn::abs(p2.y() - p1.y()) < EPSILON) {
                return bn::fixed_point(p2.x(), y);
            }
            bn::fixed t = (y - p1.y()) / (p2.y() - p1.y());
            bn::fixed x = p1.x() + t * (p2.x() - p1.x());

            return bn::fixed_point(x, y);
        }
    } // namespace math
} // namespace bna
