#include "bn_core.h"

#include "bn_bg_palettes.h"
#include "bn_colors.h"

#include "bn_vector.h"
#include "bna_hitbox.hpp"
#include "bna_vector2.hpp"
#include "bna_player.hpp"
#include "bna_enemie.hpp"
#include "bn_camera_ptr.h"
#include "bna_test_map.hpp"
#include "bna_enemies_manager.hpp"

// limite sprites 128


int main() {
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::colors::gray);
    bn::vector<bna::Enemie, 4> carros;
    carros.push_back(bna::Enemie(bn::fixed_point(60, 0)));
    carros.push_back(bna::Enemie(bn::fixed_point(90, 0)));
    carros.push_back(bna::Enemie(bn::fixed_point(120, 0)));

    bna::EnemiesManager enemiesManager(carros);

    bn::camera_ptr camara = bn::camera_ptr::create(0, 0);

    bna::TestMap map;

    bna::Player player;
    player.spawn(carros, camara, map.getSize());

    enemiesManager.spawn(carros, player, camara, map.getSize());

    while (true) {
        player.update();

        enemiesManager.update();

        bn::core::update();
    }
}
