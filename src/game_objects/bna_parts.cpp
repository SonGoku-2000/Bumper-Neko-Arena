#include "bna_parts.hpp"

// #define DEBUG
#ifdef DEBUG
#include "bn_log.h"
#endif

bna::Stats bna::parts::getMotor(MOTORS id) {
    bna::Stats stats;
    if (bna::parts::MOTORS::SLOW == id) {
        stats.maxSpeed = 2;
        stats.aceleration = 0.05;
        return stats;
    }
    else if (bna::parts::MOTORS::MEDIUM == id) {
        stats.maxSpeed = 3;
        stats.aceleration = 0.05;
        return stats;
    }
    else if (bna::parts::MOTORS::FAST == id) {
        stats.maxSpeed = 4;
        stats.aceleration = 0.05;
        return stats;
    }
#ifdef DEBUG
    BN_LOG("Motor no agregado");
#endif
    return stats;
}

bna::Stats bna::parts::getWheels(WHEELS id) {
    bna::Stats stats;
    if (bna::parts::WHEELS::NORMAL == id) {
        stats.turn = 1.5;
    }
    else if (bna::parts::WHEELS::SPIKES == id) {
        stats.turn = 2;
        stats.maxSpeed = -0.2;
    }
#ifdef DEBUG
    BN_LOG("Motor no agregado");
#endif
    return stats;
}

bna::Stats bna::parts::getBody(BODYS id) {
    bna::Stats stats;
    if (bna::parts::BODYS::LIGHT == id) {
        stats.weight = 3;
    }
    else if (bna::parts::BODYS::MEDIUM == id) {
        stats.weight = 5;
        stats.aceleration = -0.01;
    }
    else if (bna::parts::BODYS::HEAVY == id) {
        stats.weight = 8;
        stats.aceleration = -0.02;
    }
#ifdef DEBUG
    BN_LOG("Cuerpo no agregado");
#endif
    return stats;
}