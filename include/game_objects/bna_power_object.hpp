#pragma once

#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"

#include "bn_fixed_point.h"

namespace bna {
    enum class car_powers_id :int;
    class PowerObject {
        public:
        PowerObject(bn::fixed_point position, bna::car_powers_id carPower, bn::camera_ptr& camera);
        PowerObject(bn::fixed_point position, bna::car_powers_id carPower);
        ~PowerObject() = default;

        bna::car_powers_id getCarPowerId();
        void set_camera(bn::camera_ptr& camera);

        private:
        bna::car_powers_id _carPower;
        bn::sprite_ptr _sprite;
    };
} // namespace bna
