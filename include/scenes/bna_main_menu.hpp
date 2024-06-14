#include "bna_scene.hpp"

namespace bna {
    class MainMenu  : public scene {
        public:
        MainMenu();
        [[nodiscard]] bn::optional<scene_type> update() final;

        private:
    };
}
