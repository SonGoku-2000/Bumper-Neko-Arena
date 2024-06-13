#pragma once

#include "bn_size.h"
#include "bn_regular_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_vector.h"
#include "bna_hitbox.hpp"

namespace bna {
    class TestMap {
        public:
        TestMap();
        ~TestMap() = default;

        bn::size getSize();

        void setCamera(bn::camera_ptr& camera);

        bn::vector<bna::Hitbox, 4>& getWalls();

        private:
        bn::size _size;
        bn::regular_bg_ptr _fondo;

        bn::vector<bna::Hitbox, 4> _walls;
    };
} // namespace bna

