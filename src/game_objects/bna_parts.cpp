#include "bna_parts.hpp"

// #define DEBUG
#ifdef DEBUG
#include "bn_log.h"
#endif

bna::Stats bna::parts::getMotor(motors id) {
    bna::Stats stats;
    if (bna::parts::motors::SLOW == id) {
        stats.maxSpeed = 2;
        stats.aceleration = 0.05;
        return stats;
    }
    else if (bna::parts::motors::MEDIUM == id) {
        stats.maxSpeed = 3;
        stats.aceleration = 0.05;
        return stats;
    }
    else if (bna::parts::motors::FAST == id) {
        stats.maxSpeed = 4;
        stats.aceleration = 0.05;
        return stats;
    }
#ifdef DEBUG
    BN_LOG("Motor no agregado");
#endif
    return stats;
}

bna::Stats bna::parts::getWheels(wheels id) {
    bna::Stats stats;
    if (bna::parts::wheels::OLD == id) {
        stats.turn = 1;
        stats.maxSpeed = -0.2;
        stats.aceleration = -0.01;
    }
    else if (bna::parts::wheels::NORMAL == id) {
        stats.turn = 1.5;
    }
    else if (bna::parts::wheels::SPIKES == id) {
        stats.turn = 2;
        stats.maxSpeed = -0.2;
    }
#ifdef DEBUG
    BN_LOG("Motor no agregado");
#endif
    return stats;
}

bna::Stats bna::parts::getBody(bodys id) {
    bna::Stats stats;
    if (bna::parts::bodys::LIGHT == id) {
        stats.weight = 3;
    }
    else if (bna::parts::bodys::MEDIUM == id) {
        stats.weight = 5;
        stats.aceleration = -0.01;
    }
    else if (bna::parts::bodys::HEAVY == id) {
        stats.weight = 8;
        stats.aceleration = -0.02;
    }
#ifdef DEBUG
    BN_LOG("Cuerpo no agregado");
#endif
    return stats;
}