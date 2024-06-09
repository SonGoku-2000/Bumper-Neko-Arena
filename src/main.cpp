#include "bn_core.h"

#include "bn_bg_palettes.h"
#include "bn_colors.h"

#include "bn_vector.h"
#include "bna_hitbox.hpp"
#include "bna_vector2.hpp"
#include "bna_player.hpp"
#include "bna_car.hpp"
#include "bn_camera_ptr.h"
#include "bna_test_map.hpp"

// limite sprites 128


int main() {
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::colors::gray);
    bn::vector<bna::Car, 4> carros;
    carros.push_back(bna::Car(
        bna::Hitbox(bna::Vector2(60, 0), bna::Vector2(10, 20), true),
        { 60, 0 },
        5
    ));
    carros.push_back(bna::Car(
        bna::Hitbox(bna::Vector2(90, 0), bna::Vector2(10, 20), true),
        { 90, 0 },
        5
    ));

    bn::camera_ptr camara = bn::camera_ptr::create(0, 0);

    bna::TestMap map;

    bna::Player player;
    player.spawn(carros, camara, map.getSize());

    for (int i = 0; i < carros.size(); i++) {
        carros[i].spawn(camara, map.getSize());
    }

    while (true) {
        player.update();

        bn::core::update();
    }
}
