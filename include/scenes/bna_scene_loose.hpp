#pragma once

#include "bna_scene.hpp"

#include "bna_text_manager.hpp"


namespace bna {
    class SceneLoose : public scene {
        public:
        SceneLoose();
        ~SceneLoose() = default;

        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
        bna::TextManager _texto_loose;
    };
} // namespace bna
