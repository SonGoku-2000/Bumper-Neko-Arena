#include "bn_core.h"

#include "bn_colors.h"
#include "bn_bg_palettes.h"

#include "bn_unique_ptr.h"

#include "bna_scene.hpp"
#include "bna_scene_type.hpp"
#include "bna_main_menu.hpp"
#include "bna_options_menu.hpp"
#include "bna_car_selection.hpp"
#include "bna_test_map.hpp"
#include "bna_brightness_manager.hpp"
#include "bna_car_builder.hpp"
#include "bna_preparing_connection.hpp"

#ifdef DEBUG
#include "bn_log.h"
#endif

// limite sprites 128


int main() {
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::colors::gray);

    bn::unique_ptr<bna::scene> scene;

    bn::optional<bna::scene_type> next_scene = bna::scene_type::MAIN_MENU;

    bna::CarBuilder playerCar;

    int id_propia;

    while (true) {
        switch (next_scene.value()) {
            case bna::scene_type::MAIN_MENU:
                scene.reset(new bna::MainMenu());
                break;

            case bna::scene_type::PREPARING_CONNECTION:
                scene.reset(new bna::PreparingConnection(id_propia));
                break;

            case bna::scene_type::CAR_SELECTION:
                scene.reset(new bna::CarSelection(playerCar));
                break;

            case bna::scene_type::OPTION_MENU:
                scene.reset(new bna::OptionsMenu());
                break;

            case bna::scene_type::TEST_MAP:
                scene.reset(new bna::TestMap(playerCar));
                break;

            default:
                BN_ERROR("Invalid next scene: ", int(*next_scene));
                break;
        }

        next_scene = scene->update();

        bn::core::update();
    }
}
