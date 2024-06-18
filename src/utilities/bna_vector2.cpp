#include "bna_vector2.hpp"
#include "bn_sprite_ptr.h"
#include "bn_math.h"

#include "bna_math_constants.hpp"
#include "bn_log.h"

bna::Vector2::Vector2(bn::fixed x, bn::fixed y) : bn::fixed_point(x, y) {}

bna::Vector2::Vector2(bn::fixed_point vector) : bn::fixed_point(vector) {}

bna::Vector2::Vector2(bn::fixed_point p1, bn::fixed_point p2) :
    bn::fixed_point(p2.x() - p1.x(), p2.y() - p1.y()) {}

bna::Vector2::Vector2() : bn::fixed_point(0, 0) {}

bna::Vector2 bna::Vector2::rotate(bn::fixed angle) const {
    bn::fixed cosA = bn::degrees_sin(angle);
    bn::fixed sinA = bn::degrees_cos(angle);
    return Vector2(x() * cosA - y() * sinA, x() * sinA + y() * cosA);
}

bna::Vector2 bna::Vector2::rotate(bn::fixed cosTheta, bn::fixed sinTheta) const {
    bn::fixed cosA = sinTheta;
    bn::fixed sinA = cosTheta;
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
    if (len > bn::fixed(0)) {
        bn::fixed invLen = bn::fixed(1) / len; // Invertir la longitud una vez
        return Vector2(x() * invLen, y() * invLen);
    }
    return Vector2(); // Si la longitud es cero, devuelve un vector cero
}

bn::fixed bna::Vector2::angle() const {
    return  bn::degrees_atan2(int(y()), int(x()));
}

bn::fixed bna::Vector2::anglePositive() const {
    bn::fixed angulo = angle();
    if (angulo < 0) {
        return angulo + 360;
    }
    else {
        return angulo;
    }
}

bna::Vector2 bna::Vector2::withMagnitude(bn::fixed newMagnitude) const {
    Vector2 unit = normalize();
    return Vector2(unit.x() * newMagnitude, unit.y() * newMagnitude);
}
