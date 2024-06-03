#include "bna_vector2.hpp"
#include "bn_vector.h"

class Hitbox {
    public:
    Vector2 _center;
    Vector2 _size;
    bn::fixed _rotation; // en grados

    Hitbox(Vector2 center, Vector2 size, bn::fixed rotation = 0)
        : _center(center), _size(size), _rotation(rotation) {
    }

    bn::vector<Vector2,4> getVertices() const;
    bn::vector<Vector2,4> getVertices2() const;
};
