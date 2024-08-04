#include "bna_test_map_link.hpp"

#include "bn_regular_bg_items_mapa_prueba.h"

#include "bna_scene_type.hpp"

#include "bn_core.h"
#include "bna_car_builder.hpp"
#include "bna_parts.hpp"
#include "bna_car.hpp"

#include "bna_link.hpp"

#define DEBUG_CPU
#ifdef DEBUG_CPU
constexpr int CPU_CICLES = 64;
#include "bn_log.h"
#ifdef BN_CFG_PROFILER_ENABLED
#include "bn_profiler.h"
#include "bn_keypad.h"
// #define PROFILE
#endif
#endif

#include "bn_music_items.h"

bna::TestMapLink::TestMapLink(CarBuilder& player, int id_propia) :
    _fondo(bn::regular_bg_items::mapa_prueba.create_bg(0, 0)),
    _camera(bn::camera_ptr::create(0, 0)),
    _idPropia(id_propia) {
    _size = _fondo.dimensions();


    constexpr bool debug = true;
    constexpr int separacion = 10;

    _walls.push_back(bna::Hitbox(bna::Vector2(0, (_size.height() / -2) + separacion), bna::Vector2(10, _size.width() - 10), debug, 0));
    _walls.push_back(bna::Hitbox(bna::Vector2(0, (_size.height() / 2) - separacion), bna::Vector2(10, _size.width() - 10), debug, 1));
    _walls.push_back(bna::Hitbox(bna::Vector2((_size.width() / 2) - separacion, 0), bna::Vector2(_size.height() - 10, 10), debug, 2));
    _walls.push_back(bna::Hitbox(bna::Vector2((_size.width() / -2) + separacion, 0), bna::Vector2(_size.height() - 10, 10), debug, 3));
    // _walls.push_back(bna::Hitbox(bna::Vector2(60, 0), bna::Vector2(70, 10), debug, 3));
    if (id_propia == 0) {
        player.position.set_x(20);
    }
    else {
        player.position.set_x(40);
    }
    bn::vector<bna::CarBuilder, 3> carBuilders = bna::link::getCarBuilders(player);
    for (int i = 0; i < id_propia; i++) {
        if (id_propia == 0) {
            carBuilders[i].position.set_x(40);
        }
        else {
            carBuilders[i].position.set_x(20);
        }
        _cars.push_back(carBuilders[i].build());
    }
    _cars.push_back(player.build());
    _player.setBody(_cars[id_propia]);
    for (int i = id_propia; i < carBuilders.size(); i++) {
        if (id_propia == 0) {
            carBuilders[i].position.set_x(40);
        }
        else {
            carBuilders[i].position.set_x(20);
        }
        _cars.push_back(carBuilders[i].build());
    }

    _setCamera(_camera);

    for (int i = 0; i < _cars.size(); i++) {
        _cars[i].spawn(_camera, getSize());
    }

    _player.spawn(_cars, getWalls(), 0, _camera, getSize());

    bna::link::reset();
}


bn::optional<bna::scene_type> bna::TestMapLink::update() {
#ifdef DEBUG_CPU
    int cpuCont = 0;
    bn::fixed cpu = 0;
#endif
    bn::music_items::forward.play();
    bn::array<bn::optional<bna::link::fixed>, 4> mensaje_recibido;
    bn::array<bn::optional<bna::link::speed_info>, 4> mensaje_speed_data;
    int frame_actual = 0;
    while (true) {
#ifdef DEBUG_CPU
        if (cpuCont == CPU_CICLES) {
            BN_LOG("CPU : % ", cpu / CPU_CICLES * 100);
            cpu = 0;
            cpuCont = 0;
        }
        else {
            cpu += bn::core::last_cpu_usage();
            cpuCont++;
        }
#endif
#ifdef PROFILE
        if (bn::keypad::l_held() and
            bn::keypad::r_held() and
            bn::keypad::start_held()) {
            bn::profiler::show();
        }
#endif

        // for (int id_car = 0; id_car < _cars.size(); id_car++) {
        //     _cars[id_car].update(_ejes[id_car]);
        //     for (int id_wall = 0; id_wall < _walls.size(); id_wall++) {
        //         _cars[id_car].checkCollision(_walls[id_wall]);
        //     }

        //     for (int id_other = id_car + 1; id_other < _cars.size(); id_other++) {
        //         _cars[id_car].checkCollision(_cars.at(id_other));
        //     }
        // }
        for (int id_car = 0; id_car < _cars.size(); id_car++) {
            if (id_car == _idPropia) {
                _cars[_idPropia].update(_player.getEje());
            }
            else {
                _cars[id_car].update(bn::fixed_point(0, 0));
            }
            for (int id_other = id_car + 1; id_other < _cars.size(); id_other++) {
                _cars[id_car].checkCollision(_cars.at(id_other));
            }
            for (int id_wall = 0; id_wall < _walls.size(); id_wall++) {
                _cars[id_car].checkCollision(_walls[id_wall]);
            }
        }

        _sendData(frame_actual, mensaje_recibido, mensaje_speed_data);
        _updateData(mensaje_recibido, mensaje_speed_data);


        // frame_actual = bna::link::sinc(frame_actual);


        _player.update();
        bn::core::update();
    }
    return bna::scene_type::TEST_MAP;
}

void bna::TestMapLink::_sendData(int& frame_actual, bn::array<bn::optional<bna::link::fixed>, 4>& mensaje_recibido, bn::array<bn::optional<bna::link::speed_info>, 4>& mensaje_speed_data) {
    switch (frame_actual) {
        case 0:
        case 1:
        case 2:
            mensaje_recibido = bna::link::get_fixed(_cars[_idPropia].getRotation(), 0);
            break;

        case 3:
        case 4:
        case 5:
            mensaje_recibido = bna::link::get_fixed(_cars[_idPropia].getPosition().x(), 1);
            break;

        case 6:
        case 7:
        case 8:
            mensaje_recibido = bna::link::get_fixed(_cars[_idPropia].getPosition().y(), 2);
            break;

        case 9:
        case 10:
        case 11:
            mensaje_speed_data = bna::link::get_speed_info(_cars[_idPropia].getSpeed(), _cars[_idPropia].getExternalForce(), 3);
            break;

        default:
            frame_actual = -1;
            break;
    }
    frame_actual++;
}

void bna::TestMapLink::_updateData(const bn::array<bn::optional<bna::link::fixed>, 4>& mensaje_recibido, bn::array<bn::optional<bna::link::speed_info>, 4>& mensaje_speed_data) {
    for (int i = 0; i < mensaje_recibido.size(); i++) {
        if (mensaje_recibido[i].has_value()) {
            switch (mensaje_recibido[i]->fixed.id) {
                case 0:
                    _cars[i].setRotation(bn::fixed_t<4>().from_data(mensaje_recibido[i]->fixed.data));
                    break;

                case 1:
                    _cars[i].setPositionX(bn::fixed_t<4>().from_data(mensaje_recibido[i]->fixed.data));
                    break;
                case 2:
                    _cars[i].setPositionY(bn::fixed_t<4>().from_data(mensaje_recibido[i]->fixed.data));
                    break;
                default:
                    break;
            }
        }
    }

    for (int i = 0; i < mensaje_speed_data.size(); i++) {
        if (mensaje_speed_data[i].has_value()) {
            switch (mensaje_speed_data[i]->info.id) {
                case 3:
                    _cars[i].setSpeed(mensaje_speed_data[i]->info.speed);
                    _cars[i].setExternalForce(bn::fixed_point(
                        mensaje_speed_data[i]->info.external_force_x,
                        mensaje_speed_data[i]->info.external_force_y
                    ));
                    break;
                default:
                    break;
            }
        }
    }
}

void bna::TestMapLink::_setCamera(bn::camera_ptr& camera) {
    _fondo.set_camera(camera);
    for (int i = 0; i < _walls.size(); i++) {
        _walls[i].setCamera(camera);
    }
}

bn::size bna::TestMapLink::getSize() {
    return _size;
}

bn::vector<bna::Hitbox, 4>& bna::TestMapLink::getWalls() {
    return _walls;
}
