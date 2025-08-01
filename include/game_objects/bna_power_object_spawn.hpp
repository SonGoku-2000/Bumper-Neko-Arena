#pragma once

#include "bn_camera_ptr.h"
#include "bn_optional.h"

#include "bna_indicator.hpp"
#include "bn_random.h"

#include "bna_power_object.hpp"
#include "bna_hitbox.hpp"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"


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
        static bn::random _random;
        int _remainingTime;

        bn::sprite_ptr _sprite;
        bn::optional<bn::sprite_animate_action<10>> _animation;

        bn::optional<bna::PowerObject> _powerObject;

        enum state {
            EMPTY,
            OPPENING,
            READY,
            CLOSSIONG,
        };
        state _state;

        enum class fase { START, READY };
        fase _fase;

        car_powers_id _generateCarPowerId();
    };
} // namespace bna
