#include "bna_hitbox.hpp"
#include "bn_math.h"
#include "bn_sprite_items_indicador.h"
#include "bna_colissions.hpp"
#include "bn_size.h"


bna::Hitbox::Hitbox(Vector2 center, Vector2 size, bn::fixed rotation, bool debug, int color) :
    _center(center), _size(size), _rotation(rotation) {
    _vertices = _generateVertices();
    if (debug) {
        for (int i = 0; i < _spritesVertices.max_size(); i++) {
            _spritesVertices.push_back(bn::sprite_items::indicador.create_sprite(_vertices[i], color));
            _spritesVertices[i].put_above();
        }
    }
}

bna::Hitbox::Hitbox(Vector2 center, Vector2 size, bn::fixed rotation, bool debug) :
    Hitbox(center, size, rotation, debug, 0) {
}

bna::Hitbox::Hitbox(Vector2 center, Vector2 size, bool debug, int color) :
    Hitbox(center, size, 0, debug, color) {
}

bna::Hitbox::Hitbox(Vector2 center, Vector2 size, bool debug) :
    Hitbox(center, size, 0, debug, 0) {
}

bna::Hitbox::Hitbox(Vector2 center, Vector2 size, bn::fixed rotation) :
    Hitbox(center, size, rotation, false, 0) {
}

bna::Hitbox::Hitbox(Vector2 center, Vector2 size) :
    Hitbox(center, size, 0, false, 0) {
}


void bna::Hitbox::setCamera(bn::camera_ptr& camera) {
    for (int i = 0; i < _spritesVertices.size(); i++) {
        _spritesVertices[i].set_camera(camera);
    }
}

void bna::Hitbox::setRotation(bn::fixed angle) {
    _rotation = angle;
    _vertices = _generateVertices();
    _updateSpritesPos();
}

bn::fixed bna::Hitbox::getRotation() const {
    return _rotation;
}

bn::vector<bna::Vector2, 4> bna::Hitbox::getVertices() const {
    return _vertices;
}


bna::Vector2 bna::Hitbox::getCenter() const {
    return _center;
}

void bna::Hitbox::setCenter(bna::Vector2 center) {
    _center = center;
    _vertices = _generateVertices();
    _updateSpritesPos();
}

bna::Vector2 bna::Hitbox::getPosition() const {
    return _center;
}

void bna::Hitbox::setPosition(bna::Vector2 center) {
    _center = center;
    _vertices = _generateVertices();
    _updateSpritesPos();
}

void bna::Hitbox::setPosition(bn::fixed_point center) {
    _center = bna::Vector2(center.x(), center.y());
    _vertices = _generateVertices();
    _updateSpritesPos();
}


bn::size bna::Hitbox::size() const {
    return bn::size(_size.x().ceil_integer(), _size.y().ceil_integer());
}

bn::fixed bna::Hitbox::height() const {
    return size().height();
}

bn::fixed bna::Hitbox::width() const {
    return size().width();
}



bool bna::Hitbox::checkCollision(bna::Hitbox hitbox) const {
    return bna::checkCollision(*this, hitbox);
}


bna::CollisionPoint bna::Hitbox::checkCollisionPoint(Hitbox hitbox) const {
    return bna::checkCollisionPoint(*this, hitbox);
}


bn::vector<bna::Vector2, 4> bna::Hitbox::_generateVertices() const {
    bn::vector<Vector2, 4> vertices;
    Vector2 halfSize = _size * bn::fixed(0.5);

    vertices.push_back(Vector2(-halfSize.x(), -halfSize.y()).rotate(_rotation) + _center);
    vertices.push_back(Vector2(halfSize.x(), -halfSize.y()).rotate(_rotation) + _center);
    vertices.push_back(Vector2(halfSize.x(), halfSize.y()).rotate(_rotation) + _center);
    vertices.push_back(Vector2(-halfSize.x(), halfSize.y()).rotate(_rotation) + _center);

    return vertices;
}

bn::vector<bna::Vector2, 4> bna::Hitbox::_generateVertices2() const {
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


        p.set_x(p.x() - _center.x());
        p.set_y(p.y() - _center.y());

        // Rotar punto
        bn::fixed xnew = p.x() * c - p.y() * s;
        bn::fixed ynew = p.x() * s + p.y() * c;

        // Trasladar punto de vuelta
        p.set_x(xnew + _center.x());
        p.set_y(ynew + _center.y());
    }
    return vertices;
}

void bna::Hitbox::_updateSpritesPos() {
    for (int i = 0; i < _spritesVertices.size(); i++) {
        _spritesVertices[i].set_position(_vertices[i]);
    }
}
