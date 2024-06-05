#include "bn_core.h"

#include "bn_bg_palettes.h"
#include "bn_colors.h"

#include "bn_vector.h"
#include "bna_hitbox.hpp"
#include "bna_vector2.hpp"
#include "bna_player.hpp"

// limite sprites 128

int main() {
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::colors::gray);
    bn::vector<bna::Hitbox, 4> obstaculos;
    obstaculos.push_back(bna::Hitbox(bna::Vector2(32, 0), bna::Vector2(18, 10), true));
    bna::Player player;
    player.spawn(obstaculos);

    while (true) {
        player.update();

        bn::core::update();
    }
}
