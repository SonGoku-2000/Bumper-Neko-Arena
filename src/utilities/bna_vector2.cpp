#include "bna_vector2.hpp"
#include "bn_sprite_ptr.h"
#include "bn_math.h"

#include "bna_math_constants.hpp"

bna::Vector2::Vector2(bn::fixed x, bn::fixed y) : bn::fixed_point(x, y) {}

bna::Vector2::Vector2(){}

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