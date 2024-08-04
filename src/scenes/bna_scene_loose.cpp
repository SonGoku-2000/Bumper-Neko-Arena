#include "bna_scene_loose.hpp"
#include "bna_scene_type.hpp"
#include "bn_core.h"
bna::SceneLoose::SceneLoose() {
    _texto_loose.updateText("¡loose! :(");
}

[[nodiscard]] bn::optional<bna::scene_type> bna::SceneLoose::update() {
    while (true) {
        bn::core::update();
    }
    return bna::scene_type::MAIN_MENU;
}