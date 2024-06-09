#include "bna_vector2.hpp"
#include "bn_sprite_ptr.h"
#include "bn_math.h"

#include "bna_math_constants.hpp"

bna::Vector2::Vector2(bn::fixed x, bn::fixed y) : bn::fixed_point(x, y) {}

bna::Vector2::Vector2() : bn::fixed_point(0, 0) {}

bna::Vector2 bna::Vector2::rotate(bn::fixed angle) const {
    // bn::fixed rad = angle *  bn::fixed(3.14159265) / bn::fixed(180.0);
    bn::fixed cosA = degrees_sin(angle);
    bn::fixed sinA = degrees_cos(angle);
    return Vector2(x() * cosA - y() * sinA, x() * sinA + y() * cosA);
}

bna::Vector2 bna::Vector2::rotate_rad(bn::fixed rad) const {
    bn::fixed cosA = cos(rad);
    bn::fixed sinA = sin(rad);
    return Vector2(x() * cosA - y() * sinA, x() * sinA + y() * cosA);
}

bn::fixed  bna::Vector2::length() const {
    return sqrt(x() * x() + y() * y());
}

bn::fixed  bna::Vector2::squaredLength() const {
    return x() * x() + y() * y();
}

bna::Vector2  bna::Vector2::normalize() const {
    bn::fixed len = length();
    if (len != 0) {
        return Vector2(x() / len, y() / len);
    }
    return Vector2(); // Si la longitud es cero, devuelve un vector cero
}

bn::fixed bna::Vector2::angle() const {
    return bn::degrees_atan2(int(y() * 100), int(x() * 100));
}

bna::Vector2 bna::Vector2::withMagnitude(bn::fixed newMagnitude) const {
    Vector2 unit = normalize();
    return Vector2(unit.x() * newMagnitude, unit.y() * newMagnitude);
}
