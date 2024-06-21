#pragma once

#include "bn_fixed_point.h"

namespace bna {
    namespace parts {
        enum class MOTORS : int;
        enum class BODYS : int;
        enum class WHEELS :int;
    } // namespace name
    class Car;
    class CarBuilder {
        public:
        CarBuilder();
        Car build();
        parts::MOTORS motor;
        parts::BODYS body;
        parts::WHEELS wheel;
        bn::fixed_point position;
    };
} // namespace bna
