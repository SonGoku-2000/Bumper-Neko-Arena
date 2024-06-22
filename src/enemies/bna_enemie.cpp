#include "bna_enemie.hpp"

#include "bna_player.hpp"
#include "bna_car_builder.hpp"

#include "bn_math.h"
#include "bna_math.hpp"
#include "bn_display.h"

#include "bn_log.h"

#include "bn_sprite_items_indicator.h"


bna::Enemie::Enemie(CarBuilder& body) :
    _cuerpo(body.build()),

    _objetivoSprite(bn::sprite_items::indicator.create_sprite(0, 0)) {
    _vision = bn::fixed_rect(0, 0, bn::display::height() - 40, bn::display::height() - 40);
    _goingBack = false;
    _elapsedFrames = 0;
    _objetivoIdleActualizado = false;
}

void bna::Enemie::update() {
    bn::fixed_point eje(0, 0);

    if (not _goingBack) {
        int id_distancia_menor;

        _comprobarIdDistanciaMenor(id_distancia_menor);

        _comprobarAnguloObjetivo(eje, id_distancia_menor);

        // _comprobarTiempoQuieto();
    }
    else {
        if (_elapsedFrames == 96) {
            _goingBack = false;
            _elapsedFrames = 0;
        }
        else {
            _elapsedFrames++;
        }
        eje.set_y(1);
    }

    _cuerpo.update(eje);
}

void bna::Enemie::_comprobarIdDistanciaMenor(int& id_distancia_menor) {
    bn::fixed distancia_menor;
    bool distancia_menor_inicializado = false;
    bna::Vector2 vectorDistancia;
    id_distancia_menor = -2;

    _vision.set_position(_cuerpo.getPosition());
    // if (_vision.contains(_player->getPosition())) {
    //     vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _player->getPosition());
    //     vectorDistancia.set_y(-vectorDistancia.y());
    //     distancia_menor = vectorDistancia.squaredLength();

    //     id_distancia_menor = -1;
    //     distancia_menor_inicializado = true;
    // }


    for (int i = 0; i < _carros->size(); i++) {
        if (_cuerpo.getPosition() == _carros->at(i).getCar().getPosition()) {
            continue;
        }
        if (!_vision.contains(_carros->at(i).getCar().getPosition())) {
            continue;
        }

        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _carros->at(i).getCar().getPosition());

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
}

void bna::Enemie::_comprobarAnguloObjetivo(bn::fixed_point& eje, int& id_distancia_menor) {
    bn::fixed anguloObjetivo;
    bna::Vector2 vectorDistancia;
    if (id_distancia_menor == -2) {
        _comprobarObjetivoIdle();
        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _objetivoIdle);
        vectorDistancia.set_y(-vectorDistancia.y());
        anguloObjetivo = vectorDistancia.anglePositive();
    }
    else if (id_distancia_menor == -1) {
        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _player->getPosition());
        vectorDistancia.set_y(-vectorDistancia.y());
        anguloObjetivo = vectorDistancia.anglePositive();
        _objetivoIdleActualizado = false;
    }
    else {
        vectorDistancia = bna::Vector2(_cuerpo.getPosition(), _carros->at(id_distancia_menor).getCar().getPosition());
        vectorDistancia.set_y(-vectorDistancia.y());
        anguloObjetivo = vectorDistancia.anglePositive();
        _objetivoIdleActualizado = false;
    }

    eje = _direccionGiro(anguloObjetivo);
}

bn::fixed_point bna::Enemie::_direccionGiro(bn::fixed angulo_objetivo) {
    bn::fixed_point eje(0, 0);

    bn::fixed delta_theta = angulo_objetivo - _cuerpo.getRotation();
    delta_theta = (delta_theta + 180);
    delta_theta = bna::math::modulo_positivo(delta_theta, 360);
    delta_theta = delta_theta - 180;

    if (delta_theta > 0) {
        eje.set_x(1);
    }
    else if (delta_theta < 0) {
        eje.set_x(-1);
    }

    if (bn::abs(delta_theta) < 130) {
        eje.set_y(-1);
    }
    else {
        eje.set_y(1);
    }

    return eje;
}

void bna::Enemie::_comprobarTiempoQuieto() {
    _averageSpeed += {bn::abs(_cuerpo.getSpeed().x()), bn::abs(_cuerpo.getSpeed().y())};

    constexpr int framesEspera = 32;
    constexpr bn::fixed velocidadReversa = framesEspera * 0.4;
    if (_elapsedFrames == framesEspera) {
        BN_LOG(bn::abs(_averageSpeed.x()), " ", bn::abs(_averageSpeed.y()), " ", velocidadReversa);
        if (bn::abs(_averageSpeed.x()) < velocidadReversa and bn::abs(_averageSpeed.y()) < velocidadReversa) {
            _goingBack = true;
        }
        _averageSpeed = bn::fixed_point(0, 0);
        _elapsedFrames = 0;
    }
    else {
        _elapsedFrames++;
    }
}

void bna::Enemie::_comprobarObjetivoIdle() {
    if (not _objetivoIdleActualizado) {
        _nuevoObjetivoIdle();
        return;
    }
    bn::fixed disX = bn::abs(_cuerpo.getPosition().x() - _objetivoIdle.x());
    bn::fixed disY = bn::abs(_cuerpo.getPosition().y() - _objetivoIdle.y());

    constexpr int proximidadCambio = 20;
    if (disX <= proximidadCambio and disY <= proximidadCambio) {
        _nuevoObjetivoIdle();
    }
}

void bna::Enemie::_nuevoObjetivoIdle() {
    constexpr int SEPARACION_BORDES = 60;
    int widht = (_limitesEscenario.width() - SEPARACION_BORDES) / 2;
    int height = (_limitesEscenario.height() - SEPARACION_BORDES) / 2;
    _objetivoIdle.set_x(_random.get_int(-widht, widht));
    _objetivoIdle.set_y(_random.get_int(-height, height));
    _objetivoIdleActualizado = true;
    _objetivoSprite.set_position(_objetivoIdle);
}

void bna::Enemie::spawn(bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& carros, bna::Player& player, bn::vector<bna::Hitbox, 4>& walls, bn::camera_ptr& camera, bn::size size) {
    _carros = &carros;
    _player = &player;
    _walls = &walls;
    _cuerpo.spawn(camera, size);
    _limitesEscenario = size;
    _objetivoSprite.set_camera(camera);
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