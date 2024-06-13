#include "bna_enemies_manager.hpp"
#include "bn_log.h"


bna::EnemiesManager::EnemiesManager(bn::vector<bna::Enemie, 4>& enemies) :
    _enemies(&enemies) {

}


void bna::EnemiesManager::update() {
    for (int i = 0; i < _enemies->size(); ++i) {
        _enemies->at(i).update();
        for (int j = i + 1; j < _enemies->size(); ++j) {
            _enemies->at(i).checkCollision(_enemies->at(j).getCar());
        }

        for (int j = 0; j < _walls->size(); j++) {
            _enemies->at(i).checkCollision(_walls->at(j));
        }
    }
}

void bna::EnemiesManager::spawn(bn::vector<bna::Enemie, 4>& carros, bna::Player& player, bn::vector<bna::Hitbox, 4>& walls, bn::camera_ptr& camera, bn::size size) {
    for (int i = 0; i < _enemies->size(); i++) {
        _enemies->at(i).spawn(carros, player, walls, camera, size);
    }

    _walls = &walls;
}
