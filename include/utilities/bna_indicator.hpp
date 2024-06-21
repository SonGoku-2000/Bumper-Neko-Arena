#pragma once
#include "bn_fixed_point.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"

namespace bna {
    class Indicator : public bn::fixed_point {
        public:
        Indicator(bn::fixed x, bn::fixed y, bool visible, int color = 7);
        Indicator(bn::fixed_point pos, bool visible, int color = 7);
        ~Indicator() = default;
        void set_camera(bn::camera_ptr camara);

        private:
        bn::optional<bn::sprite_ptr> _sprite;
    };
} // namespace rs
