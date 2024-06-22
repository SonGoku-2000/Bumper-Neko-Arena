#pragma once

#include "bna_car.hpp"
#include "bn_random.h"

#include "bna_test_values.hpp"

#include "bn_fixed_rect.h"
#include "bn_random.h"

#include "bn_sprite_ptr.h"

namespace bna {
    class Player;
    class CarBuilder;
    class Enemie {
        public:
        Enemie(CarBuilder& body);
        ~Enemie() = default;

        void update();
        void spawn(bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& carros, bna::Player& player, bn::vector<bna::Hitbox, 4>& walls, bn::camera_ptr& camera, bn::size size);
        void checkCollision(bna::Car& car);
        void checkCollision(bna::Enemie& car);
        void checkCollision(bna::Hitbox& hitbox);

        bna::Car& getCar();

        private:
        bna::Car _cuerpo;
        bn::fixed_rect _vision;

        bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>* _carros;
        bna::Player* _player;
        bn::vector<bna::Hitbox, 4>* _walls;

        bn::random _random;

        bn::size _limitesEscenario;
        bool _goingBack;
        bn::fixed_point _averageSpeed;
        int _elapsedFrames;
        bn::point _objetivoIdle;
        bn::sprite_ptr _objetivoSprite;
        bool _objetivoIdleActualizado;

        void _comprobarIdDistanciaMenor(int& id_distancia_menor);
        void _comprobarAnguloObjetivo(bn::fixed_point& eje, int& id_distancia_menor);
        bn::fixed_point _direccionGiro(bn::fixed angulo_objetivo);
        void _comprobarTiempoQuieto();
        void _comprobarObjetivoIdle();
        void _nuevoObjetivoIdle();
    };
} // namespace bna
