#include "bna_main_menu.hpp"

#include "bna_scene_type.hpp"


bna::MainMenu::MainMenu() {

}


bn::optional<bna::scene_type> bna::MainMenu::update() {
    return bna::scene_type::TEST_MAP;
}

