#include "bna_position_icon_manager.hpp"

#include "bna_enemie.hpp"

#include "bn_sprite_items_icons.h"
#include "bn_sprite_items_icons_back.h"
#include "bn_fixed_point.h"

#include "bna_math.hpp"
#include "bna_display.hpp"
#include "bna_vector2.hpp"


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
        _iconsBack.push_back(bn::sprite_items::icons_back.create_sprite(0, 0));
        _icons.push_back(bn::sprite_items::icons.create_sprite(i * 16, i * 16, int(_enemies->at(i).getCharacter())));
    }
}


void bna::PositionIconManager::update() {
    bn::fixed_point objetivo;
    for (int i = 0; i < _enemies->size(); i++) {
        objetivo = _enemies->at(i).getPosition() - _camera.position();

        objetivo = _getIconPosition(objetivo);

        _icons[i].set_position(objetivo);
        _iconsBack[i].set_position(objetivo);
        _iconsBack[i].set_tiles(bn::sprite_items::icons_back.tiles_item(), _getIconFrame(objetivo));
    }
}

bn::fixed_point bna::PositionIconManager::_getIconPosition(bn::fixed_point car_objetive) {
    if (car_objetive.x() > 0) {
        car_objetive = bna::math::interpolate_x(bn::fixed_point(0, 0), car_objetive, _limite.right());
        if (car_objetive.y() > _limite.bottom()) {
            car_objetive = bna::math::interpolate_y(bn::fixed_point(0, 0), car_objetive, _limite.bottom());
        }
        else if (car_objetive.y() < _limite.top()) {
            car_objetive = bna::math::interpolate_y(bn::fixed_point(0, 0), car_objetive, _limite.top());
        }
    }
    else if (car_objetive.x() < 0) {
        car_objetive = bna::math::interpolate_x(bn::fixed_point(0, 0), car_objetive, _limite.left());
        if (car_objetive.y() > _limite.bottom()) {
            car_objetive = bna::math::interpolate_y(bn::fixed_point(0, 0), car_objetive, _limite.bottom());
        }
        else if (car_objetive.y() < _limite.top()) {
            car_objetive = bna::math::interpolate_y(bn::fixed_point(0, 0), car_objetive, _limite.top());
        }
    }
    return car_objetive;
}

int bna::PositionIconManager::_getIconFrame(bna::Vector2 car_objetive) {
    car_objetive.set_y(-car_objetive.y());
    bn::fixed angulo = car_objetive.anglePositive();
    return angulo.division(10).integer();
}
