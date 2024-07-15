#include "bna_icon_roulette.hpp"

bna::IconRoulette::IconRoulette(bn::fixed_point position, bn::fixed separation, bn::vector<bn::sprite_ptr, 3> icons) {
    _icons = icons;
    _positions[0] = Indicator(position - bn::fixed_point(separation, 0), true);
    _positions[1] = Indicator(position, true);
    _positions[2] = Indicator(position + bn::fixed_point(separation, 0), true);

    for (int i = 0; i < _icons.size(); i++) {
        _icons[i].set_position(_positions[i]);
    }
}