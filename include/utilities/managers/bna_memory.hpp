#pragma once

#include "bn_array.h"
#include "bn_vector.h"
#include "bn_optional.h"
#include "bn_random.h"
#include "bna_car_builder.hpp"

namespace bna {
    class Memory {
        public:
        constexpr static const char* valid_label = "bna_01";

        char label[8] = {};

        bn::fixed brillo;

        Memory();
        ~Memory() = default;

        [[nodiscard]] bool read();

        void write();

        private:
    };
}
