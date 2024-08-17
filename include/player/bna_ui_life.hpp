#pragma once

#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "bn_optional.h"

namespace bna {
    class Car;
    enum class CharactersId :int;
    class UILife {
        public:
        UILife();
        ~UILife() = default;

        void update();
        void setCar(bna::Car& car);

        private:
        bn::optional<bn::sprite_ptr> _catFace;
        bn::optional<bn::sprite_animate_action<8>> _catAnimation;
        bn::optional<bn::sprite_ptr> _catFace2;
        bn::optional<bn::sprite_animate_action<8>> _catAnimation2;

        bn::sprite_ptr _healthBar;
        bna::Car* _car;

        CharactersId _catId;
        bn::fixed _previousLife;

        void _resetAnimation();
        void _setSprite();
    };
} // namespace bna
