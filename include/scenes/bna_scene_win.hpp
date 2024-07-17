#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"


namespace bna {
    class SceneWin : public scene {
        public:
        SceneWin();
        ~SceneWin() = default;

        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        bna::TextManager _texto_win;
    };
} // namespace bna
