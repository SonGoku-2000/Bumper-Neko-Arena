#pragma once

#include "bna_vector2.hpp"
#include "bn_vector.h"

namespace bna {
    class Hitbox {
        public:
        Hitbox(Vector2 center, Vector2 size, bn::fixed rotation = 0);

        bn::vector<Vector2, 4> getVertices() const;
        bn::vector<Vector2, 4> getVertices2() const;

        void setRotation(bn::fixed angle);
        bn::fixed getRotation();

        void setCenter(Vector2 center);
        Vector2 getCenter();

        private:
        Vector2 _center;
        Vector2 _size;
        bn::fixed _rotation; // en grados
    };
} // namespace bna
