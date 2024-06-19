#include "bna_hitbox.hpp"
#include "bn_math.h"
#include "bn_sprite_items_indicator.h"
#include "bna_colissions.hpp"
#include "bn_size.h"


bna::Hitbox::Hitbox(Vector2 center, Vector2 size, bn::fixed rotation, bool debug, int color) :
    _center(center), _size(size), _rotation(rotation),
    _axesNormalized(_vertices.max_size()),
    _axes(_vertices.max_size()) {
    _vertices = _generateVertices();
    _axesNormalidedUpdated = false;
    _axesUpdated = false;

    for (int i = 0; i < 4; i++) {
        _projectionsInfo[i].updated = false;
        _projectionsNormalizedInfo[i].updated = false;
    }

    if (debug) {
        for (int i = 0; i < _spritesVertices.max_size(); i++) {
            _spritesVertices.push_back(bn::sprite_items::indicator.create_sprite(_vertices[i], color));
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


bn::vector<bna::Vector2, 4> bna::Hitbox::_generateVertices() const {
    bn::vector<Vector2, 4> vertices;
    Vector2 halfSize = _size * bn::fixed(0.5);

    bn::fixed cosTheta2 = bn::degrees_cos(_rotation);
    bn::fixed sinTheta2 = bn::degrees_sin(_rotation);

    vertices.push_back(Vector2(-halfSize.x(), -halfSize.y()).rotate(cosTheta2, sinTheta2) + _center);
    vertices.push_back(Vector2(halfSize.x(), -halfSize.y()).rotate(cosTheta2, sinTheta2) + _center);
    vertices.push_back(Vector2(halfSize.x(), halfSize.y()).rotate(cosTheta2, sinTheta2) + _center);
    vertices.push_back(Vector2(-halfSize.x(), halfSize.y()).rotate(cosTheta2, sinTheta2) + _center);

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


void bna::Hitbox::setCamera(bn::camera_ptr& camera) {
    for (int i = 0; i < _spritesVertices.size(); i++) {
        _spritesVertices[i].set_camera(camera);
    }
}

void bna::Hitbox::setRotation(bn::fixed angle) {
    if (_rotation != angle) {
        _rotation = angle;
        _axesNormalidedUpdated = false;
        _axesUpdated = false;
        _vertices = _generateVertices();
        _updateSpritesPos();
        for (int i = 0; i < 4; i++) {
            _projectionsInfo[i].updated = false;
            _projectionsNormalizedInfo[i].updated = false;

        }
    }
}

bn::fixed bna::Hitbox::getRotation() const {
    return _rotation;
}

bn::vector<bna::Vector2, 4> bna::Hitbox::getVertices() const {
    return _vertices;
}

bn::vector<bna::Vector2, 4> bna::Hitbox::getAxesNormalized() {
    if (!_axesNormalidedUpdated) {
        _axesNormalidedUpdated = true;
        int size = _vertices.size();
        for (int i = 0; i < size; i++) {
            const bna::Vector2& p1 = _vertices[i];
            const bna::Vector2& p2 = _vertices[(i + 1) % size];
            bna::Vector2 edge = p2 - p1;
            bna::Vector2 normal = bna::Vector2(-edge.y(), edge.x()).normalize();
            _axesNormalized[i] = normal; // Normal al borde
        }
    }
    return _axesNormalized;
}

bn::vector<bna::Vector2, 4> bna::Hitbox::getAxes() {
    if (!_axesUpdated) {
        _axesUpdated = true;
        int size = _vertices.size();
        for (int i = 0; i < size; i++) {
            const bna::Vector2& p1 = _vertices[i];
            const bna::Vector2& p2 = _vertices[(i + 1) % size];
            bna::Vector2 edge = p2 - p1;
            bna::Vector2 normal = bna::Vector2(-edge.y(), edge.x());
            _axes[i] = normal; // Normal al borde
        }
    }
    return _axes;
}

std::pair<bn::fixed, bn::fixed> bna::Hitbox::getProjection(int self_axis_id) {
    if (_projectionsInfo[self_axis_id].updated) {
        return _projectionsInfo[self_axis_id].projection;
    }
    else {
        const bna::Vector2& axis = _axes[self_axis_id];
        _projectionsInfo[self_axis_id].projection = getProjection(axis);
        _projectionsInfo[self_axis_id].updated = true;
        return _projectionsInfo[self_axis_id].projection;
    }
}

std::pair<bn::fixed, bn::fixed> bna::Hitbox::getProjectionNormalized(int self_axis_id) {
    if (_projectionsNormalizedInfo[self_axis_id].updated) {
        return _projectionsNormalizedInfo[self_axis_id].projection;
    }
    else {
        const bna::Vector2& axis = _axesNormalized[self_axis_id];
        _projectionsNormalizedInfo[self_axis_id].projection = getProjection(axis);
        _projectionsNormalizedInfo[self_axis_id].updated = true;
        return _projectionsNormalizedInfo[self_axis_id].projection;
    }
}

std::pair<bn::fixed, bn::fixed> bna::Hitbox::getProjection(const bna::Vector2& axis) {
    bn::fixed min = _vertices[0].dot(axis);
    bn::fixed max = min;
    for (const bna::Vector2& vertex : _vertices) {
        bn::fixed projection = vertex.dot(axis);
        if (projection < min) {
            min = projection;
        }
        else if (projection > max) {
            max = projection;
        }
    }
    return std::make_pair(min, max);
}


bna::Vector2 bna::Hitbox::getCenter() const {
    return _center;
}

void bna::Hitbox::setCenter(bna::Vector2 center) {
    if (_center != center) {
        bn::fixed_point offset = center - _center;
        _axesNormalidedUpdated = false;
        _axesUpdated = false;
        _center = center;
        for (int i = 0; i < _vertices.size(); i++) {
            _vertices[i] = _vertices[i] + offset;
        }
        _updateSpritesPos();

        for (int i = 0; i < 4; i++) {
            _projectionsInfo[i].updated = false;
            _projectionsNormalizedInfo[i].updated = false;
        }
    }
}

bna::Vector2 bna::Hitbox::getPosition() const {
    return _center;
}

void bna::Hitbox::setPosition(bna::Vector2 center) {
    setCenter(center);
}

void bna::Hitbox::setPosition(bn::fixed_point center) {
    setCenter(bna::Vector2(center.x(), center.y()));
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



bool bna::Hitbox::checkCollision(bna::Hitbox hitbox) {
    return bna::checkCollision(*this, hitbox);
}

bna::CollisionPoint bna::Hitbox::checkCollisionPoint(Hitbox& hitbox) {
    return bna::checkCollisionPointV2(*this, hitbox);
}

