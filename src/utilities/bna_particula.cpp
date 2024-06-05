#include "bna_particula.hpp"

#include "bn_math.h"

Particle::Particle(bn::fixed new_x, bn::fixed new_y, bn::fixed new_vx, bn::fixed new_vy, bn::fixed new_radius, bn::fixed new_mass)
    : x(new_x), y(new_y), vx(new_vx), vy(new_vy), radius(new_radius), mass(new_mass) {
}

void Particle::move() {
    x += vx * TIME_STEP;
    y += vy * TIME_STEP;

    // Handle boundary collisions
    if (x - radius < 0 || x + radius > WIDTH / 2) {
        vx = -vx;
    }
    if (y - radius < 0 || y + radius > HEIGHT / 2) {
        vy = -vy;
    }
}

bool Particle::isColliding(const Particle& other) const {
    bn::fixed dx = x - other.x;
    bn::fixed dy = y - other.y;
    bn::fixed distance = bn::sqrt(dx * dx + dy * dy);
    return distance < (radius + other.radius);
}

void Particle::resolveCollision(Particle& other) {
    bn::fixed dx = x - other.x;
    bn::fixed dy = y - other.y;
    bn::fixed distance = bn::sqrt(dx * dx + dy * dy);

    if (distance == 0) return;

    // Normal vector
    bn::fixed nx = dx / distance;
    bn::fixed ny = dy / distance;

    // Relative velocity
    bn::fixed dvx = vx - other.vx;
    bn::fixed dvy = vy - other.vy;
    bn::fixed dotProduct = dvx * nx + dvy * ny;

    // If the particles are moving apart, no need to resolve collision
    if (dotProduct > 0) return;

    // Impulse scalar
    bn::fixed impulse = 2 * dotProduct / (mass + other.mass);  // mass is 1 for both

    // Change velocities
    vx -= impulse * other.mass * nx;
    vy -= impulse * other.mass * ny;
    other.vx += impulse * mass * nx;
    other.vy += impulse * mass * ny;
}
