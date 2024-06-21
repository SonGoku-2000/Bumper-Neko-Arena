#include "bna_enemie.hpp"

#include "bna_player.hpp"

#include "bna_car_builder.hpp"

bna::Enemie::Enemie(CarBuilder& body) :
    _cuerpo(body.build()) {
}

void bna::Enemie::update() {
    bn::fixed_point eje(0, 0);

    bn::fixed distancia_menor;
    bna::Vector2 vectorDistancia;
    int id_distancia_menor;

    vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _player->getPosition());
    vectorDistancia.set_y(-vectorDistancia.y());


    distancia_menor = vectorDistancia.squaredLength();
    id_distancia_menor = -1;
    for (int i = 0; i < _carros->size(); i++) {
        if (_cuerpo.getPosition() == _carros->at(i).getCar().getPosition()) {
            continue;
        }
        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _carros->at(i).getCar().getPosition());
        vectorDistancia.set_y(-vectorDistancia.y());

        if (distancia_menor > vectorDistancia.squaredLength()) {
            distancia_menor = vectorDistancia.squaredLength();
            id_distancia_menor = i;
        }
    }
    if (id_distancia_menor == -1) {
        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _player->getPosition());
    }
    else {
        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _carros->at(id_distancia_menor).getCar().getPosition());
    }
    vectorDistancia.set_y(-vectorDistancia.y());

    bn::fixed anguloObjetivo = vectorDistancia.anglePositive();
    if (_cuerpo.getRotation() < anguloObjetivo) {
        eje.set_x(1);
    }
    else if (_cuerpo.getRotation() > anguloObjetivo) {
        eje.set_x(-1);
    }

    eje.set_y(-1);

    _cuerpo.update(eje);
}

void bna::Enemie::spawn(bn::vector<bna::Enemie, 4>& carros, bna::Player& player, bn::vector<bna::Hitbox, 4>& walls, bn::camera_ptr& camera, bn::size size) {
    _carros = &carros;
    _player = &player;
    _walls = &walls;
    _cuerpo.spawn(camera, size);
}

void bna::Enemie::checkCollision(bna::Car& car) {
    _cuerpo.checkCollision(car);
}

void bna::Enemie::checkCollision(bna::Hitbox& hitbox) {
    _cuerpo.checkCollision(hitbox);
}

void bna::Enemie::checkCollision(bna::Enemie& car) {
    car.getCar().checkCollision(_cuerpo);
}

bna::Car& bna::Enemie::getCar() {
    return _cuerpo;
}