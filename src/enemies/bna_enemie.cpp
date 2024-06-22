#include "bna_enemie.hpp"

#include "bna_player.hpp"

#include "bna_car_builder.hpp"

#include "bn_display.h"


bna::Enemie::Enemie(CarBuilder& body) :
    _cuerpo(body.build()) {
    _vision = bn::fixed_rect(0, 0, bn::display::height() - 40, bn::display::height() - 40);
}

void bna::Enemie::update() {
    bn::fixed_point eje(0, 0);

    bn::fixed distancia_menor;
    bool distancia_menor_inicializado = false;
    bna::Vector2 vectorDistancia;
    int id_distancia_menor;
    id_distancia_menor = -2;

    _vision.set_position(_cuerpo.getPosition());

    if (_vision.contains(_player->getPosition())) {
        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _player->getPosition());
        vectorDistancia.set_y(-vectorDistancia.y());
        distancia_menor = vectorDistancia.squaredLength();

        id_distancia_menor = -1;
        distancia_menor_inicializado = true;
    }


    for (int i = 0; i < _carros->size(); i++) {
        if (_cuerpo.getPosition() == _carros->at(i).getCar().getPosition()) {
            continue;
        }
        if (!_vision.contains(_carros->at(i).getCar().getPosition())) {
            continue;
        }

        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _carros->at(i).getCar().getPosition());
        vectorDistancia.set_y(-vectorDistancia.y());

        if (!distancia_menor_inicializado) {
            distancia_menor = vectorDistancia.squaredLength();
            id_distancia_menor = i;
            distancia_menor_inicializado = true;
        }
        else if (distancia_menor > vectorDistancia.squaredLength()) {
            distancia_menor = vectorDistancia.squaredLength();
            id_distancia_menor = i;
        }
    }

    bn::fixed anguloObjetivo;
    if (id_distancia_menor == -2) {
        anguloObjetivo = _random.get_int(360);
    }
    else if (id_distancia_menor == -1) {
        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _player->getPosition());
        vectorDistancia.set_y(-vectorDistancia.y());
        anguloObjetivo = vectorDistancia.anglePositive();
    }
    else {
        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _carros->at(id_distancia_menor).getCar().getPosition());
        vectorDistancia.set_y(-vectorDistancia.y());
        anguloObjetivo = vectorDistancia.anglePositive();
    }

    if (_cuerpo.getRotation() < anguloObjetivo) {
        eje.set_x(1);
    }
    else if (_cuerpo.getRotation() > anguloObjetivo) {
        eje.set_x(-1);
    }

    eje.set_y(-1);

    _cuerpo.update(eje);
}

void bna::Enemie::spawn(bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& carros, bna::Player& player, bn::vector<bna::Hitbox, 4>& walls, bn::camera_ptr& camera, bn::size size) {
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