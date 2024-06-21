#include "bna_car_builder.hpp"

#include "bna_parts.hpp"
#include "bna_car.hpp"

bna::CarBuilder::CarBuilder() {
}

bna::Car bna::CarBuilder::build() {
    bna::Stats stats;
    stats = stats + bna::parts::getBody(body);
    stats = stats + bna::parts::getMotor(motor);
    stats = stats + bna::parts::getWheels(wheel);
    return bna::Car(bna::Hitbox(bna::Vector2(position), bna::Vector2(10, 20), true), position, stats);
}