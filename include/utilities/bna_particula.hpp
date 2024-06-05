#pragma once

#include "bn_fixed.h"

#include "bn_display.h"

const bn::fixed WIDTH = bn::display::width();
const bn::fixed HEIGHT = bn::display::height();
const bn::fixed TIME_STEP = 0.1;

class Particle {
    public:
    bn::fixed x, y;
    bn::fixed vx, vy;
    bn::fixed radius;
    bn::fixed mass;

    Particle(bn::fixed x, bn::fixed y, bn::fixed vx, bn::fixed vy, bn::fixed radius,bn::fixed mass);

    void move();

    bool isColliding(const Particle& other) const;

    void resolveCollision(Particle& other);
};

