#pragma once

#include "bn_optional.h"

#include "bna_indicator.hpp"
#include "bn_random.h"

#include "bna_power_object.hpp"

namespace bna {
    enum class car_powers_id :int;
    class PowerObjectSpawn {
        public:
        PowerObjectSpawn(bn::fixed_point position);
        ~PowerObjectSpawn() = default;

        void update();

        private:
        bna::Indicator _position;
        bn::random _random;
        int _remainingTime;
        bn::optional<bna::PowerObject> _powerObject;

        car_powers_id _generateCarPowerId();
    };
} // namespace bna
