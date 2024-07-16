#include "bna_ui_life.hpp"

#include "bn_sprite_items_life_bar.h"

#include "bna_test_values.hpp"
#include "bna_car.hpp"

bna::UILife::UILife() :
    _sprite(bn::sprite_items::life_bar.create_sprite(-85, -60)) {

}

void bna::UILife::setCar(bna::Car& car) {
    _car = &car;
}

void bna::UILife::update() {
    if (_car->getLife() > (bna::limit_values::MAX_LIFE / 4) * 3) {
        _sprite.set_item(bn::sprite_items::life_bar, 0);
    }
    else  if (_car->getLife() > (bna::limit_values::MAX_LIFE / 4) * 2) {
        _sprite.set_item(bn::sprite_items::life_bar, 1);
    }
    else  if (_car->getLife() > bna::limit_values::MAX_LIFE / 4) {
        _sprite.set_item(bn::sprite_items::life_bar, 2);
    }
    else if (_car->getLife() > 0) {
        _sprite.set_item(bn::sprite_items::life_bar, 3);
    }
    else {
        _sprite.set_item(bn::sprite_items::life_bar, 4);
    }
}

