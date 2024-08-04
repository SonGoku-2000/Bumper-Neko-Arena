#pragma once

#include "bn_array.h"
#include "bn_vector.h"
#include "bn_optional.h"
#include "bn_random.h"

#include "bn_array.h"

namespace bna {
    namespace parts {
        enum class motors : int;
        enum class bodys : int;
        enum class wheels :int;
    } // namespace parts

    class Memory {
        public:
        constexpr static const char* valid_label = "bna_01";

        char label[8] = {};

        bn::fixed brillo;
        bn::array<parts::motors, 3> motores;
        bn::array<parts::bodys, 3> cuerpos;
        bn::array<parts::wheels, 3> ruedas;

        Memory();
        ~Memory() = default;

        [[nodiscard]] bool read();

        void write();

        private:
    };
}
