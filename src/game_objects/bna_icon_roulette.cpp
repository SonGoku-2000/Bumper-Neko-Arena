#include "bna_icon_roulette.hpp"

bna::IconRoulette::IconRoulette(bn::fixed_point position, bn::fixed separation) {
    _positions[0] = Indicator(position - bn::fixed_point(separation, 0), true);
    _positions[1] = Indicator(position, true);
    _positions[2] = Indicator(position + bn::fixed_point(separation, 0), true);
}