#pragma once

#include "bn_vector.h"
#include "bn_array.h"

#include "bn_sprite_ptr.h"
#include "bn_optional.h"

#include "bna_indicator.hpp"


namespace bna {
    class IconRoulette {
        public:
        IconRoulette(bn::fixed_point position, bn::fixed separation, bn::array<bn::sprite_ptr, 3> icons, int initial_option);
        IconRoulette(bn::fixed_point position, bn::fixed separation, bn::array<bn::sprite_ptr, 3> icons);
        IconRoulette(bn::fixed_point position, bn::fixed separation);
        IconRoulette();
        ~IconRoulette() = default;

        int getSelection();
        void setIcons(bn::array<bn::sprite_ptr, 3> icons);

        void netxOption();
        void previousOption();

        private:
        bn::array<bn::optional<bn::sprite_ptr>, 3> _icons;
        bn::array<bna::Indicator, 3> _positions;
        int _selection;

        void _updateIconsPosition();
    };
} // namespace bna
