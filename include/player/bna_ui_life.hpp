#pragma once

#include "bn_sprite_ptr.h"


namespace bna {
    class Car;
    class UILife {
        public:
        UILife();
        ~UILife() = default;

        void update();
        void setCar(bna::Car& car);

        private:
        bn::sprite_ptr _sprite;
        bna::Car* _car;
    };
} // namespace bna
