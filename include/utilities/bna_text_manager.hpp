#pragma once

#include "bn_sprite_text_generator.h"
#include "bn_sprite_palette_ptr.h"

#include "bn_optional.h"
#include "bn_fixed.h"
#include "bn_sprite_ptr.h"
#include "bn_string.h"
#include "bn_vector.h"

namespace bna {
    class TextManager {
        public:
        TextManager();
        TextManager(bn::fixed x, bn::fixed y);
        TextManager(bn::fixed x, bn::fixed y, bn::string<111> text);
        TextManager(bn::fixed x, bn::fixed y, bn::string<111> text, bn::sprite_font font);
        TextManager(bn::fixed x, bn::fixed y, bn::string<111> text, bn::sprite_font font, int split);

        void updateText(bn::string<111> texto);
        void updateText(bn::string<111> texto, const int split);
        void setVisible(bool visible);
        int length();
        bn::string<111> getText();
        void set_position(bn::fixed_point pos);
        void set_position(bn::fixed x, bn::fixed y);
        void set_x(bn::fixed x);
        void set_y(bn::fixed y);

        void set_aligment(bn::sprite_text_generator::alignment_type alignment);
        void set_palette_item(bn::sprite_palette_item palette);
        void reset_palette_item();

        private:
        bn::fixed _posX;
        bn::fixed _posY;
        bn::sprite_text_generator _text_generator;
        bn::vector<bn::sprite_ptr, 111> _text_sprites;
        bn::string<111> _text;
        bn::optional<bn::sprite_palette_item> _originalPalette;

        /**
         * @brief Retorna un porcion del string ingresado limitado por [inicio, final].
         * @param inicio Inicio del slice incluido.
         * @param final Final del slice incluido.
         */
        template<int MaxSize>
        bn::string<MaxSize> slice(bn::string<MaxSize> string, int inicio, int final);

    };
}
