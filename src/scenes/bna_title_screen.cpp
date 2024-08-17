#include "bna_scene_type.hpp"
#include "bna_title_screen.hpp"
#include "bn_core.h"

#include "bn_regular_bg_items_main_screen.h"

#include "bn_keypad.h"


bna::TitleScreen::TitleScreen() :
    _background(bn::regular_bg_items::main_screen.create_bg(0, 0)) {
    _textStart = bna::TextManager(0, 70, "Press Start");
    _textStart.set_aligment(bn::sprite_text_generator::alignment_type::CENTER);
}

bn::optional<bna::scene_type> bna::TitleScreen::update() {
    int frames = 0;
    int time_visible = 60;
    int time_hide = 40;
    while (true) {
        if (bn::keypad::any_pressed()) {
            return bna::scene_type::MAIN_MENU;
        }

        if ((_textStart.visible() && frames == time_visible) ||
            (!_textStart.visible() && frames == time_hide)) {
            _textStart.setVisible(!_textStart.visible());
            frames = 0;
        }
        else {
            frames++;
        }

        bn::core::update();
    }
}