#include "bna_enemie.hpp"


bna::Enemie::Enemie(bn::fixed_point pos) :
    _cuerpo(bna::Hitbox(bna::Vector2(pos), bna::Vector2(10, 20), true), pos, 5) {

}

void bna::Enemie::update() {
    _cuerpo.update({ _random.get_int(-1,2),_random.get_int(-1,2) });
}

void bna::Enemie::spawn(bn::vector<bna::Enemie, 4>& carros, bna::Player& player, bn::camera_ptr& camera, bn::size size) {
    _carros = &carros;
    _player = &player;
    _cuerpo.spawn(camera, size);
}

void bna::Enemie::checkCollision(bna::Car& car) {
    _cuerpo.checkCollision(car);
}

bna::Car& bna::Enemie::getCar() {
    return _cuerpo;
}