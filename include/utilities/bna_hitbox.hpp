#pragma once

#include "bna_vector2.hpp"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"

namespace bna {
    class Hitbox {
        public:
        Hitbox(Vector2 center, Vector2 size);
        Hitbox(Vector2 center, Vector2 size, bn::fixed rotation);
        Hitbox(Vector2 center, Vector2 size, bn::fixed rotation, bool debug);

        bn::vector<Vector2, 4> getVertices() const;
        bn::vector<Vector2, 4> getVertices2();

        void setRotation(bn::fixed angle);
        bn::fixed getRotation();

        void setCenter(Vector2 center);
        Vector2 getCenter();

        private:
        Vector2 _center;
        Vector2 _size;
        bn::fixed _rotation; // en grados

        bn::vector<bn::sprite_ptr, 4> _spritesVertices;
        bn::vector<Vector2, 4> _vertices;

        bn::vector<Vector2, 4> _generateVertices() const;
        bn::vector<Vector2, 4> _generateVertices2() const;
    };
} // namespace bna
