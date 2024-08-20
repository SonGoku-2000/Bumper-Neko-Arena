#include "bna_gba_jam_logo.hpp"

#include "bn_core.h"
#include "bna_scene_type.hpp"

#include "bn_regular_bg_items_gbajam24_logo.h"

#include "bn_blending.h"



bna::GbaJamLogo::GbaJamLogo() :
    _background(bn::regular_bg_items::gbajam24_logo.create_bg(0, 0)) {
    _estado = estados::APARECIENDO;
}

bn::optional<bna::scene_type> bna::GbaJamLogo::update() {
    _background.set_blending_enabled(true);
    bn::blending::set_fade_alpha(1);
    int elapsedFrames = 0;

    while (true) {
        bn::fixed fade_alpha = bn::blending::fade_alpha();
        switch (_estado) {
            case estados::APARECIENDO: {
                bn::blending::set_fade_alpha(bn::max(fade_alpha - 0.02, bn::fixed(0)));
                if (bn::blending::fade_alpha() == 0) {
                    _estado = estados::ESPERANDO_VISIBLE;
                }
                break;
            }
            case estados::ESPERANDO_VISIBLE: {
                if (elapsedFrames == 120) {
                    elapsedFrames = 0;
                    _estado = estados::DESEPARECIENDO;
                }
                elapsedFrames++;
                break;
            }
            case estados::DESEPARECIENDO: {
                bn::blending::set_fade_alpha(bn::min(fade_alpha + 0.02, bn::fixed(1)));
                if (bn::blending::fade_alpha() == 1) {
                    _estado = estados::ESPERANDO_OCULTO;
                }
                break;
            }
            case estados::ESPERANDO_OCULTO: {
                if (elapsedFrames == 60) {
                    return bna::scene_type::TITLE_SCREEN;
                }
                elapsedFrames++;
                break;
            }
            default:
                break;
        }
        bn::core::update();
    }
}