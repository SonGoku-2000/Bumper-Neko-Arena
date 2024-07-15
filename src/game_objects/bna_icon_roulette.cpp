#include "bna_icon_roulette.hpp"

#include "bna_loop_value.hpp"

namespace bna {
    bool MOSTRAR_INDICADORES = false;
} // namespace bna


bna::IconRoulette::IconRoulette(bn::fixed_point position, bn::fixed separation, bn::array<bn::sprite_ptr, 3> icons, int initial_option) {
    setIcons(icons);
    _positions[0] = Indicator(position - bn::fixed_point(separation, 0), MOSTRAR_INDICADORES);
    _positions[1] = Indicator(position, MOSTRAR_INDICADORES);
    _positions[2] = Indicator(position + bn::fixed_point(separation, 0), MOSTRAR_INDICADORES);

    _selection = initial_option;
    _updateIconsPosition();
}

bna::IconRoulette::IconRoulette(bn::fixed_point position, bn::fixed separation, bn::array<bn::sprite_ptr, 3> icons) :
    IconRoulette(position, separation, icons, 0) {
}

bna::IconRoulette::IconRoulette(bn::fixed_point position, bn::fixed separation) {
    _positions[0] = Indicator(position - bn::fixed_point(separation, 0), MOSTRAR_INDICADORES);
    _positions[1] = Indicator(position, MOSTRAR_INDICADORES);
    _positions[2] = Indicator(position + bn::fixed_point(separation, 0), MOSTRAR_INDICADORES);

    _selection = 0;
}

bna::IconRoulette::IconRoulette() {
}




void bna::IconRoulette::setIcons(bn::array<bn::sprite_ptr, 3> icons) {
    for (int i = 0; i < icons.size(); i++) {
        _icons[i] = icons[i];
    }
    _updateIconsPosition();
}

int bna::IconRoulette::getSelection() {
    return _selection;
}


void bna::IconRoulette::netxOption() {
    _selection = bna::loop(_selection + 1, 0, _icons.size() - 1);
    _updateIconsPosition();
}

void bna::IconRoulette::previousOption() {
    _selection = bna::loop(_selection - 1, 0, _icons.size() - 1);
    _updateIconsPosition();
}

void bna::IconRoulette::_updateIconsPosition() {
    _icons[bna::loop(_selection - 1, 0, _icons.size() - 1)]->set_position(_positions[0]);
    _icons[_selection]->set_position(_positions[1]);
    _icons[bna::loop(_selection + 1, 0, _icons.size() - 1)]->set_position(_positions[2]);
}

