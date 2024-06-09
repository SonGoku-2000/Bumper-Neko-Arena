#pragma once

#include "bn_size.h"

namespace bna {
    class TestMap {
        public:
        TestMap();
        ~TestMap() = default;

        bn::size getSize();

        private:
        bn::size _size;
    };
} // namespace bna

