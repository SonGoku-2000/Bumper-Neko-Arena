#include "bna_scene_loose.hpp"
#include "bna_scene_type.hpp"
#include "bn_core.h"
#include "bn_keypad.h"


bna::SceneLoose::SceneLoose() {
    _texto_loose.updateText("¡loose! :(");
    _texto_continue.set_y(30);
    _texto_continue.updateText("start to continue");
}

[[nodiscard]] bn::optional<bna::scene_type> bna::SceneLoose::update() {
    while (true) {
        bn::core::update();
        if (bn::keypad::start_pressed()) {
            return bna::scene_type::MAIN_MENU;
        }
    }
    return bna::scene_type::MAIN_MENU;
}