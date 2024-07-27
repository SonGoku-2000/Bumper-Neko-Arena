#include "bna_scene_win.hpp"
#include "bna_scene_type.hpp"
#include "bn_core.h"
bna::SceneWin::SceneWin() {
    _texto_win.updateText("¡Win!");
}

[[nodiscard]] bn::optional<bna::scene_type> bna::SceneWin::update() {
    while (true) {
        bn::core::update();
    }
    return bna::scene_type::MAIN_MENU;
}