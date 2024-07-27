#pragma once

#include "bn_camera_ptr.h"
#include "bna_camera_manager.hpp"

#include "bna_hitbox.hpp"
#include "bna_vector2.hpp"
#include "bn_size.h"

#include "bn_vector.h"
#include "bn_optional.h"

#include "bna_car.hpp"
#include "bna_enemie.hpp"

#include "bna_test_values.hpp"

namespace bna {
    class CarBuilder;
    enum class Characters :int;
    enum class car_powers_id :int;
    class Player {
        public:
        Player();
        ~Player() = default;

        void full_update();
        void spawn(bn::vector<bna::Car, limit_values::MAX_ENEMIES + 1>& cars, bn::vector<bna::Hitbox, 4>& walls, int id_propia, bn::camera_ptr& camera, bn::size size);

        bn::fixed_point getPosition();

        void setBody(Car& body);
        void setCharacter(const Characters& character);
        bna::Car& getCarRef();
        bna::Vector2 getEje();
        void update();

        void givePower(car_powers_id carPower);

        private:
        bna::Vector2 _eje;

        bn::vector<bna::Car, limit_values::MAX_ENEMIES + 1>* _enemies;

        bna::Car* _cuerpo;
        bna::Characters _character;

        bn::optional<bna::CameraManager> _cameraManager;

        bn::vector<bna::Hitbox, 4>* _walls;

        int _idPropia;
        car_powers_id _carPower;
    };
} // namespace bna

