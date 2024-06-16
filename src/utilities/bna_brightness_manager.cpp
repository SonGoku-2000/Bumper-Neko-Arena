#include "bna_brightness_manager.hpp"

#include "bn_bg_palettes.h"
#include "bn_sprite_palettes.h"


constexpr bn::fixed MAX_BRIGHTNESS = 0.4;

bn::fixed bna::brightness_manager::get_brightness() {
    return bn::bg_palettes::brightness();
}

void bna::brightness_manager::set_brightness(bn::fixed brightness) {
    bn::fixed new_brightness = bn::min(MAX_BRIGHTNESS, bn::max(brightness, bn::fixed(0)));
    bn::bg_palettes::set_brightness(new_brightness);
    bn::sprite_palettes::set_brightness(new_brightness);
}