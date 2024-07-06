#include "bna_position_icon_manager.hpp"

#include "bna_enemie.hpp"

#include "bn_sprite_items_icons.h"
#include "bn_fixed_point.h"

#include "bna_math.hpp"
#include "bna_display.hpp"


namespace bna {
    constexpr int BORDE = 80;
} // namespace bna


bna::PositionIconManager::PositionIconManager(bn::camera_ptr& camera, bn::vector<bna::Enemie, limit_values::MAX_ENEMIES>& enemies) :
    _camera(camera),
    _limite(bn::point(0, 0), bn::display::size() - bn::size(BORDE, BORDE)) {
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
            diferencia = bna::math::interpolate_x(bn::fixed_point(0, 0), diferencia, _limite.right());
            if (diferencia.y() > _limite.bottom()) {
                diferencia = bna::math::interpolate_y(bn::fixed_point(0, 0), diferencia, _limite.bottom());
            }
            else if (diferencia.y() < _limite.top()) {
                diferencia = bna::math::interpolate_y(bn::fixed_point(0, 0), diferencia, _limite.top());
            }
        }
        else if (diferencia.x() < 0) {
            diferencia = bna::math::interpolate_x(bn::fixed_point(0, 0), diferencia, _limite.left());
            if (diferencia.y() > _limite.bottom()) {
                diferencia = bna::math::interpolate_y(bn::fixed_point(0, 0), diferencia, _limite.bottom());
            }
            else if (diferencia.y() < _limite.top()) {
                diferencia = bna::math::interpolate_y(bn::fixed_point(0, 0), diferencia, _limite.top());
            }
        }
        _icons[i].set_position(diferencia);
    }
}
