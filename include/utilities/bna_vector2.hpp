#pragma once
#include "bn_fixed_point.h"

class Vector2 :public bn::fixed_point {
    public:

    Vector2();
    Vector2(bn::fixed x, bn::fixed y);

    Vector2 operator*(bn::fixed scalar) const {
        return Vector2(x() * scalar, y() * scalar);
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x() + other.x(), y() + other.y());
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x() - other.x(), y() - other.y());
    }
    // constexpr operator bn::fixed_point() {
    //     return bn::fixed_point(*this);
    // }

    Vector2 rotate(bn::fixed angle) const;
    Vector2 rotate_rad(bn::fixed rad) const;
};
