#pragma once

#include "bn_camera_ptr.h"
#include "bn_optional.h"

#include "bna_indicator.hpp"
#include "bn_random.h"

#include "bna_power_object.hpp"
#include "bna_hitbox.hpp"


namespace bna {
    class Hitbox;
    enum class car_powers_id :int;
    class PowerObjectSpawn {
        public:
        PowerObjectSpawn(bn::fixed_point position, bn::camera_ptr& camera);
        ~PowerObjectSpawn() = default;

        void update();

        bool checkColission(bna::Hitbox& otherHitbox);
        
        car_powers_id takePower();

        private:
        bna::Indicator _position;
        bn::camera_ptr _camera;
        bn::random _random;
        int _remainingTime;

        bn::optional<bna::PowerObject> _powerObject;

        car_powers_id _generateCarPowerId();
    };
} // namespace bna
