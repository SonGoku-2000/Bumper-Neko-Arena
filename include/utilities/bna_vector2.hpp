#pragma once
#include "bn_fixed_point.h"
#include "bn_fixed.h"

namespace bna {
    class Vector2 :public bn::fixed_point {
        public:

        Vector2();
        Vector2(bn::fixed x, bn::fixed y);
        Vector2(bn::fixed_point p1, bn::fixed_point p2);
        Vector2(bn::fixed_point vector);

        Vector2 operator*(bn::fixed scalar) const {
            return Vector2(x() * scalar, y() * scalar);
        }

        Vector2 operator*(int scalar) const {
            return Vector2(x() * scalar, y() * scalar);
        }

        Vector2 operator+(const Vector2& other) const {
            return Vector2(x() + other.x(), y() + other.y());
        }

        Vector2 operator+(const bn::fixed_point& other) const {
            return Vector2(x() + other.x(), y() + other.y());
        }

        Vector2 operator-(const Vector2& other) const {
            return Vector2(x() - other.x(), y() - other.y());
        }
        Vector2 operator-(const bn::fixed_point& other) const {
            return Vector2(x() - other.x(), y() - other.y());
        }
        // constexpr operator bn::fixed_point() {
        //     return bn::fixed_point(*this);
        // }

        bn::fixed dot(const Vector2& other) const {
            return x() * other.x() + y() * other.y();
        }

        Vector2 rotate(bn::fixed angle) const;
        Vector2 rotate_rad(bn::fixed rad) const;

        bn::fixed length() const;
        bn::fixed squaredLength() const;

        Vector2 normalize() const;

        bn::fixed angle() const;
        bn::fixed anglePositive() const;

        Vector2 withMagnitude(bn::fixed newMagnitude) const;
    };
} // namespace bna
