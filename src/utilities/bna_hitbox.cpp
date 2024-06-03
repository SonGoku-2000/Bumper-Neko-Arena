#include "bna_hitbox.hpp"
#include "bn_math.h"

bn::vector<Vector2, 4> Hitbox::getVertices() const {
    bn::vector<Vector2, 4> vertices;
    Vector2 halfSize = _size * bn::fixed(0.5);

    vertices.push_back(Vector2(-halfSize.x(), -halfSize.y()).rotate(_rotation) + _center);
    vertices.push_back(Vector2(halfSize.x(), -halfSize.y()).rotate(_rotation) + _center);
    vertices.push_back(Vector2(halfSize.x(), halfSize.y()).rotate(_rotation) + _center);
    vertices.push_back(Vector2(-halfSize.x(), halfSize.y()).rotate(_rotation) + _center);

    return vertices;
}

bn::vector<Vector2, 4> Hitbox::getVertices2() const {
    bn::vector<Vector2, 4> vertices;
    vertices.push_back({ 10,10 });
    vertices.push_back({ 10,-10 });
    vertices.push_back({ -10,-10 });
    vertices.push_back({ -10,10 });
    bn::fixed s = degrees_sin(_rotation);
    bn::fixed c = degrees_cos(_rotation);

    // Trasladar punto al origen
    for (int i = 0; i < vertices.size(); i++) {
        bn::fixed_point& p = vertices[i];
        /* code */


        p.set_x(p.x()-_center.x());
        p.set_y(p.y()-_center.y());

        // Rotar punto
        bn::fixed xnew = p.x() * c - p.y() * s;
        bn::fixed ynew = p.x() * s + p.y() * c;

        // Trasladar punto de vuelta
        p.set_x( xnew + _center.x());
        p.set_y( ynew + _center.y());
    }
    return vertices;
}