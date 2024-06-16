#include "bna_text_manager.hpp"

#include "common_variable_8x16_sprite_font.h"

#include "bn_sprite_ptr.h"

#include "bna_planes.hpp"


bna::TextManager::TextManager() :
    TextManager(0, -65, "", common::variable_8x16_sprite_font, 0) {
}

bna::TextManager::TextManager(bn::fixed x, bn::fixed y) :
    TextManager(x, y, "", common::variable_8x16_sprite_font, 0) {
}

bna::TextManager::TextManager(bn::fixed x, bn::fixed y, bn::string<111> texto) :
    TextManager(x, y, texto, common::variable_8x16_sprite_font, 0) {
}

bna::TextManager::TextManager(bn::fixed x, bn::fixed y, bn::string<111> texto, bn::sprite_font font) :
    TextManager(x, y, texto, font, 0) {
}

bna::TextManager::TextManager(bn::fixed x, bn::fixed y, bn::string<111> texto, bn::sprite_font font, int split) :
    _text_generator(font){
    _posX = x;
    _posY = y;
    _text_generator.set_bg_priority(bna::Planes::FIRST);
    _text_generator.set_alignment(bn::sprite_text_generator::alignment_type::LEFT);
    updateText(texto, split);
    _originalPalette = font.item().palette_item();
}



void bna::TextManager::updateText(bn::string<111> texto) {
    updateText(texto, 0);
}

void bna::TextManager::updateText(bn::string<111> texto, const int split) {
    _text_sprites.clear();
    _text = texto;
    if (split == 0) {
        _text_generator.generate(_posX, _posY, _text, _text_sprites);
    }
    else  if (_text.size() < split) {
        _text_generator.generate(_posX, _posY, _text, _text_sprites);
    }
    else {
        int contador = 0;
        bn::string<111> textoReducido;

        while (contador * split < _text.size()) {
            textoReducido = slice(texto, contador * split, ((contador + 1) * split));
            _text_generator.generate(_posX, (21 * contador) + _posY, textoReducido, _text_sprites);
            contador++;
        }
    }
}


void bna::TextManager::set_aligment(bn::sprite_text_generator::alignment_type alignment) {
    _text_generator.set_alignment(alignment);
    updateText(_text);
}

void bna::TextManager::set_palette_item(bn::sprite_palette_item palette) {
    _text_generator.set_palette_item(palette);
    updateText(_text);
}

void bna::TextManager::reset_palette_item(){
    _text_generator.set_palette_item(_originalPalette.value());
    updateText(_text);
}

void bna::TextManager::setVisible(bool visible) {
    for (bn::sprite_ptr letra : _text_sprites) {
        letra.set_visible(visible);
    }
}

int bna::TextManager::length() {
    return _text.length();
}

bn::string<111> bna::TextManager::getText() {
    return _text;
}

void bna::TextManager::set_position(bn::fixed_point pos) {
    bn::fixed_point diferencia = pos - bn::fixed_point(_posX, _posY);
    _posX = pos.x();
    _posY = pos.y();
    for (int i = 0; i < _text_sprites.size(); i++) {
        _text_sprites[i].set_position(_text_sprites[i].position() + diferencia);
    }
}

void bna::TextManager::set_position(bn::fixed x, bn::fixed y) {
    set_position({ x,y });
}

void bna::TextManager::set_x(bn::fixed x) {
    set_position({ x,_posY });
}

void bna::TextManager::set_y(bn::fixed y) {
    set_position({ _posX,y });
}



template<int MaxSize>
bn::string<MaxSize> bna::TextManager::slice(bn::string<MaxSize> string, int inicio, int final) {
    return bn::string<MaxSize>(string.begin() + inicio, string.begin() + bn::min(final, string.size()));
}

