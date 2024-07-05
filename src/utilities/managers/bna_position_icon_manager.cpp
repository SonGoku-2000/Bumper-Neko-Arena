#include "bna_position_icon_manager.hpp"

#include "bna_enemie.hpp"

#include "bn_sprite_items_icons.h"
#include "bn_fixed_point.h"

bna::PositionIconManager::PositionIconManager(bn::camera_ptr& camera, bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& enemies) :
    _camera(camera) {
    _enemies = &enemies;
}

void bna::PositionIconManager::generateIcons(){
    for (int i = 0; i < _enemies->size(); i++) {
        _icons.push_back(bn::sprite_items::icons.create_sprite(i*16, i*16,int(_enemies->at(i).getCharacter())));
    }
}


void bna::PositionIconManager::update() {
    bn::fixed_point diferencia;
    for (int i = 0; i < _enemies->size(); i++) {
// diferencia = _enemies->at(i).;
    }
}
