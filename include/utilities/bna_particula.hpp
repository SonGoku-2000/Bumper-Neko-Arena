#pragma once

#include "bn_fixed.h"

#include "bn_display.h"

#include "bn_vector.h"

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

void update(bn::vector<Particle, 2>& particulas);
