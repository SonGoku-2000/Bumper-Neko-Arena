#pragma once

#include "bn_size.h"
#include "bn_regular_bg_ptr.h"
#include "bn_camera_ptr.h"

namespace bna {
    class TestMap {
        public:
        TestMap();
        ~TestMap() = default;

        bn::size getSize();

        void setCamera(bn::camera_ptr& camera);

        private:
        bn::size _size;
        bn::regular_bg_ptr _fondo;
    };
} // namespace bna

