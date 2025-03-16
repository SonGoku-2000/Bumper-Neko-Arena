#include "bna_ui_power.hpp"

#include "bn_sprite_items_health_bar_power_icon.h"
#include "bna_planes.hpp"
#include "bna_player.hpp"

bna::UIPower::UIPower() :
    _power_icon(bn::sprite_items::health_bar_power_icon.create_sprite(-48, -60)) {
    _power_icon.set_bg_priority(Planes::FIRST);
}

void bna::UIPower::set_player(bna::Player& player) {
    _player = &player;
    _set_sprite();
}

void bna::UIPower::_set_sprite() {
    _power_icon.set_item(bn::sprite_items::health_bar_power_icon, int(_player->get_car_power()));
}

void bna::UIPower::update() {
    _set_sprite();
}