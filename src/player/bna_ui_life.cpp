#include "bna_ui_life.hpp"

#include "bn_sprite_items_life_bar.h"
#include "bn_sprite_items_health_bar.h"

#include "bn_sprite_items_health_bar_cat_black.h"
#include "bn_sprite_items_health_bar_cat_persian.h"
#include "bn_sprite_items_health_bar_cat_siamese.h"
#include "bn_sprite_items_health_bar_cat_tricolour_face.h"
#include "bn_sprite_items_health_bar_cat_tricolour_hat.h"

#include "bna_test_values.hpp"
#include "bna_car.hpp"

#include "bna_characters_id.hpp"
#include "bna_planes.hpp"


bna::UILife::UILife() :
    _healthBar(bn::sprite_items::health_bar.create_sprite(-85, -50)) {
    _healthBar.set_bg_priority(Planes::FIRST);
}

void bna::UILife::setCar(bna::Car& car) {
    _car = &car;
    _catId = _car->getCharacterId();
    _setSprite();
    _resetAnimation();
}

void bna::UILife::update() {
    // if (_car->getLife() > (bna::limit_values::MAX_LIFE / 4) * 3) {
    //     _sprite.set_item(bn::sprite_items::life_bar, 0);
    // }
    // else  if (_car->getLife() > (bna::limit_values::MAX_LIFE / 4) * 2) {
    //     _sprite.set_item(bn::sprite_items::life_bar, 1);
    // }
    // else  if (_car->getLife() > bna::limit_values::MAX_LIFE / 4) {
    //     _sprite.set_item(bn::sprite_items::life_bar, 2);
    // }
    // else if (_car->getLife() > 0) {
    //     _sprite.set_item(bn::sprite_items::life_bar, 3);
    // }
    // else {
    //     _sprite.set_item(bn::sprite_items::life_bar, 4);
    // }
}

void bna::UILife::_setSprite() {
    if (bna::CharactersId::BLACK == _catId) {
        bn::fixed_point offset(0, 0);
        _catFace = bn::sprite_items::health_bar_cat_black.create_sprite(_healthBar.position() + offset);
    }
    if (bna::CharactersId::PERSIAN == _catId) {
        bn::fixed_point offset(0, 2);
        _catFace = bn::sprite_items::health_bar_cat_persian.create_sprite(_healthBar.position() + offset);
    }
    if (bna::CharactersId::SIAMESE == _catId) {
        bn::fixed_point offset(-1, 5);
        _catFace = bn::sprite_items::health_bar_cat_siamese.create_sprite(_healthBar.position() + offset);
    }
    if (bna::CharactersId::TRICOLOUR == _catId) {
        bn::fixed_point offset(0, 0);
        _catFace = bn::sprite_items::health_bar_cat_tricolour_face.create_sprite(_healthBar.position() + offset);
        _catFace2 = bn::sprite_items::health_bar_cat_tricolour_hat.create_sprite(_healthBar.position() + offset);
    }

    _catFace->set_bg_priority(Planes::FIRST);
    _catFace->put_above();
    if (_catFace2.has_value()) {
        _catFace2->set_bg_priority(Planes::FIRST);
        _catFace2->put_above();
    }
}

void bna::UILife::_resetAnimation() {

}
