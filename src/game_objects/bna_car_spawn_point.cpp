#include "bna_car_spawn_point.hpp"


bna::CarSpawnPoint::CarSpawnPoint(bna::Indicator new_indicator, bn::fixed new_rotation) {
    indicator = new_indicator;
    rotation = new_rotation;
}

bna::CarSpawnPoint::CarSpawnPoint() {
}