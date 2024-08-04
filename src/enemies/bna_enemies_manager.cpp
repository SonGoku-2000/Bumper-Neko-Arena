#include "bna_enemies_manager.hpp"

#ifdef DEBUG
#include "bn_log.h"
#endif


bna::EnemiesManager::EnemiesManager(bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& enemies) :
    _enemies(&enemies) {

}


void bna::EnemiesManager::update() {
    for (int i = 0; i < _enemies->size(); ++i) {
        _enemies->at(i).update();
    }
}

void bna::EnemiesManager::spawn(bn::vector<bna::Car, limit_values::MAX_ENEMIES + 1>& carros, bn::vector<bna::Hitbox, 4>& walls, bn::camera_ptr& camera, bn::size size) {
    for (int i = 0; i < _enemies->size(); i++) {
        _enemies->at(i).spawn(carros, walls, camera, size);
    }

    _walls = &walls;
}

void bna::EnemiesManager::givePower(bna::car_powers_id carPower, int id) {
    _enemies->at(id - 1).givePower(carPower);
}
