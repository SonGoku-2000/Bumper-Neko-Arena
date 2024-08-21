#pragma once 

#include "bna_indicator.hpp"
#include "bn_fixed.h"


namespace bna {
    struct CarSpawnPoint {
        CarSpawnPoint(bna::Indicator new_indicator, bn::fixed new_rotation);
        CarSpawnPoint();
        ~CarSpawnPoint() = default;

        bna::Indicator indicator;
        bn::fixed rotation;
    };
} // namespace bna
