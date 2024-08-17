#pragma once

#include "bn_fixed_point.h"

namespace bna {
    namespace parts {
        enum class motors : int;
        enum class bodys : int;
        enum class wheels :int;
    } // namespace name
    enum class CharactersId :int;
    class Car;
    class CarBuilder {
        public:
        CarBuilder();
        Car build();
        parts::motors motor;
        parts::bodys body;
        parts::wheels wheel;
        bn::fixed_point position;
        CharactersId cat_id;
    };
} // namespace bna
