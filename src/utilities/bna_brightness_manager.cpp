#include "bna_brightness_manager.hpp"

#include "bn_bg_palettes.h"
#include "bn_sprite_palettes.h"

#include "bn_log.h"

bn::fixed bna::brightness_manager::get_brightness() {
    return bn::bg_palettes::brightness();
}

int bna::brightness_manager::get_brightness_percent() {
    bn::fixed brillo = bna::brightness_manager::get_brightness();
    brillo = brillo.multiplication(100);
    brillo = brillo.division(bna::brightness_manager::MAX_BRIGHTNESS);
    return bn::min(brillo.floor_integer(), 100);
}

void bna::brightness_manager::set_brightness_percent(int brighness) {
    bn::fixed brillo = bna::brightness_manager::MAX_BRIGHTNESS.multiplication(brighness);
    brillo = brillo.division(100);
    set_brightness(brillo);
}

void bna::brightness_manager::set_brightness(bn::fixed brightness) {
    bn::fixed new_brightness = bn::min(MAX_BRIGHTNESS, bn::max(brightness, bn::fixed(0)));
    bn::bg_palettes::set_brightness(new_brightness);
    bn::sprite_palettes::set_brightness(new_brightness);
}