#include "bna_position_icon_manager.hpp"

#include "bna_enemie.hpp"

#include "bn_sprite_items_icons.h"
#include "bn_fixed_point.h"

#include "bna_math.hpp"
#include "bna_display.hpp"


namespace bna {
    constexpr int BORDE = 20;
} // namespace bna


bna::PositionIconManager::PositionIconManager(bn::camera_ptr& camera, bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& enemies) :
    _camera(camera) {
    _enemies = &enemies;
}

void bna::PositionIconManager::generateIcons() {
    for (int i = 0; i < _enemies->size(); i++) {
        _icons.push_back(bn::sprite_items::icons.create_sprite(i * 16, i * 16, int(_enemies->at(i).getCharacter())));
    }
}


void bna::PositionIconManager::update() {
    bn::fixed_point diferencia;
    for (int i = 0; i < _enemies->size(); i++) {
        diferencia = _enemies->at(i).getPosition() - _camera.position();

        if (diferencia.x() > 0) {
            diferencia = bna::math::interpolate_x(bn::fixed_point(0, 0), diferencia, bn::display::half_width() - BORDE);
            if (diferencia.y() > bn::display::half_height() - BORDE) {
                diferencia = bna::math::interpolate_y(bn::fixed_point(0, 0), diferencia, bn::display::half_height() - BORDE);
            }
            else if (diferencia.y() < -bn::display::half_height() + BORDE) {
                diferencia = bna::math::interpolate_y(bn::fixed_point(0, 0), diferencia, -bn::display::half_height() + BORDE);
            }
        }
        else if (diferencia.x() < 0) {
            diferencia = bna::math::interpolate_x(bn::fixed_point(0, 0), diferencia, -bn::display::half_width() + BORDE);
            if (diferencia.y() > bn::display::half_height() - BORDE) {
                diferencia = bna::math::interpolate_y(bn::fixed_point(0, 0), diferencia, bn::display::half_height() - BORDE);
            }
            else if (diferencia.y() < -bn::display::half_height() + BORDE) {
                diferencia = bna::math::interpolate_y(bn::fixed_point(0, 0), diferencia, -bn::display::half_height() + BORDE);
            }
        }
        _icons[i].set_position(diferencia);
    }
}
