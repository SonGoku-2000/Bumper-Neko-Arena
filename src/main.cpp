#include "bn_core.h"

#include "bn_colors.h"
#include "bn_bg_palettes.h"

#include "bn_unique_ptr.h"

#include "bna_scene.hpp"
#include "bna_scene_type.hpp"
#include "bna_main_menu.hpp"
#include "bna_options_menu.hpp"
#include "bna_car_selection.hpp"
#include "bna_car_selection_multiplayer.hpp"
#include "bna_test_map.hpp"
#include "bna_test_map_link.hpp"
#include "bna_scene_win.hpp"
#include "bna_scene_loose.hpp"
#include "bna_brightness_manager.hpp"
#include "bna_car_builder.hpp"
#include "bna_characters_id.hpp"
#include "bna_preparing_connection.hpp"
#include "bna_character_selection.hpp"

#include "bna_memory.hpp"
#define DEBUG
#ifdef DEBUG
#include "bn_log.h"
#endif

// limite sprites 128


int main() {
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::colors::gray);
    bna::Memory memoria;

    bn::unique_ptr<bna::scene> scene;

    bn::optional<bna::scene_type> next_scene = bna::scene_type::MAIN_MENU;

    bna::CarBuilder playerCar;
    bna::CharactersId& playerCharacter = playerCar.cat_id;

    int id_propia;

    while (true) {
        switch (next_scene.value()) {
            case bna::scene_type::MAIN_MENU:
                scene.reset(new bna::MainMenu());
                break;

            case bna::scene_type::PREPARING_CONNECTION:
                scene.reset(new bna::PreparingConnection(id_propia));
                break;

            case bna::scene_type::CHARACTER_SELECTION:
                scene.reset(new bna::CharacterSelection(playerCharacter));
                break;

            case bna::scene_type::CAR_SELECTION:
                scene.reset(new bna::CarSelection(playerCar, memoria.motores, memoria.cuerpos, memoria.ruedas));
                break;

            case bna::scene_type::CAR_SELECTION_MULTIPLAYER:
                scene.reset(new bna::CarSelectionMultiplayer(playerCar));
                break;

            case bna::scene_type::OPTION_MENU:
                scene.reset(new bna::OptionsMenu(memoria.brillo));
                break;

            case bna::scene_type::TEST_MAP:
                scene.reset(new bna::TestMap(playerCar, playerCharacter));
                break;

            case bna::scene_type::TEST_MAP_LINK:
                scene.reset(new bna::TestMapLink(playerCar, id_propia));
                break;

            case bna::scene_type::SCENE_WIN:
                scene.reset(new bna::SceneWin(memoria.motores, memoria.cuerpos, memoria.ruedas));
                break;

            case bna::scene_type::SCENE_LOOSE:
                scene.reset(new bna::SceneLoose());
                break;

            default:
                BN_ERROR("Invalid next scene: ", int(*next_scene));
                break;
        }

        next_scene = scene->update();

        memoria.write();

        bn::core::update();
    }
}
