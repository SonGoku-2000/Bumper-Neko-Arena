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
    // _resetAnimation();
    _previousLife = bna::limit_values::MAX_LIFE;
}

void bna::UILife::update() {
    if (_catAnimation.has_value()) {
        if (!_catAnimation->done()) {
            _catAnimation->update();
            if (_catAnimation2.has_value()) {
                _catAnimation2->update();
            }
        }
    }

    if (_car->getLife() < _previousLife) {
        _resetAnimation();
        _previousLife = _car->getLife();
    }

    _shakeBar();

    if (_car->getLife() > (bna::limit_values::MAX_LIFE / 7) * 6) {
        _healthBar.set_item(bn::sprite_items::health_bar, 0);
    }
    else if (_car->getLife() > (bna::limit_values::MAX_LIFE / 7) * 5) {
        _healthBar.set_item(bn::sprite_items::health_bar, 1);
    }
    else if (_car->getLife() > (bna::limit_values::MAX_LIFE / 7) * 4) {
        _healthBar.set_item(bn::sprite_items::health_bar, 2);
    }
    else if (_car->getLife() > (bna::limit_values::MAX_LIFE / 7) * 3) {
        _healthBar.set_item(bn::sprite_items::health_bar, 3);
    }
    else if (_car->getLife() > (bna::limit_values::MAX_LIFE / 7) * 2) {
        _healthBar.set_item(bn::sprite_items::health_bar, 4);
    }
    else if (_car->getLife() > (bna::limit_values::MAX_LIFE / 7)) {
        _healthBar.set_item(bn::sprite_items::health_bar, 5);
    }
    else {
        _healthBar.set_item(bn::sprite_items::health_bar, 6);
    }
}

void bna::UILife::_shakeBar() {
    if (!_catAnimation.has_value()) {
        return;
    }
    if (_catAnimation->done()) {
        return;
    }

    int offset = 1;

    if (bna::CharactersId::SIAMESE == _catId) {
        offset = -1;
    }
    if (_catAnimation->current_index() == 0 || _catAnimation->current_index() == 1|| _catAnimation->current_index() == 2) {
        _healthBar.set_x(_catFace->x() + offset);
    }
    else {
        _healthBar.set_x(_catFace->x());
    }
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
    if (bna::CharactersId::BLACK == _catId) {
        _catAnimation = bn::create_sprite_animate_action_once(_catFace.value(), 8, bn::sprite_items::health_bar_cat_black.tiles_item(), 1, 1, 2, 2, 0);
    }
    if (bna::CharactersId::PERSIAN == _catId) {
        _catAnimation = bn::create_sprite_animate_action_once(_catFace.value(), 8, bn::sprite_items::health_bar_cat_persian.tiles_item(), 1, 1, 2, 3, 4, 5, 0);
    }
    if (bna::CharactersId::SIAMESE == _catId) {
        _catAnimation = bn::create_sprite_animate_action_once(_catFace.value(), 8, bn::sprite_items::health_bar_cat_siamese.tiles_item(), 1, 1, 2, 3, 4, 5, 0);
    }
    if (bna::CharactersId::TRICOLOUR == _catId) {
        _catAnimation = bn::create_sprite_animate_action_once(_catFace.value(), 8, bn::sprite_items::health_bar_cat_tricolour_face.tiles_item(), 1, 1, 2, 3, 0);
        _catAnimation2 = bn::create_sprite_animate_action_once(_catFace2.value(), 8, bn::sprite_items::health_bar_cat_tricolour_hat.tiles_item(), 1, 1, 2, 3, 0);
    }
}
