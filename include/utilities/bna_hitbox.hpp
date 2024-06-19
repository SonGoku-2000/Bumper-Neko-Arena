#pragma once

#include "bna_vector2.hpp"
#include "bn_vector.h"
#include "bn_array.h"

#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"


namespace bna {
    struct CollisionPoint;
    struct ProjectionInfo;
    class Hitbox {
        public:
        Hitbox(Vector2 center, Vector2 size, bn::fixed rotation, bool debug, int color);
        Hitbox(Vector2 center, Vector2 size, bn::fixed rotation, bool debug);
        Hitbox(Vector2 center, Vector2 size, bool debug, int color);
        Hitbox(Vector2 center, Vector2 size, bool debug);
        Hitbox(Vector2 center, Vector2 size, bn::fixed rotation);
        Hitbox(Vector2 center, Vector2 size);

        bn::vector<Vector2, 4> getVertices() const;

        void setRotation(bn::fixed angle);
        bn::fixed getRotation() const;

        void setCenter(Vector2 center);
        Vector2 getCenter() const;

        void setPosition(Vector2 position);
        void setPosition(bn::fixed_point position);
        Vector2 getPosition() const;

        bn::size size() const;
        bn::fixed width() const;
        bn::fixed height() const;

        bool checkCollision(Hitbox hitbox) const;
        CollisionPoint checkCollisionPoint(Hitbox& hitbox);

        void setCamera(bn::camera_ptr& camera);

        bn::vector<bna::Vector2, 4> getAxesNormalized();

        std::pair<bn::fixed, bn::fixed> getProjection(int self_axis_id);
        std::pair<bn::fixed, bn::fixed> getProjection(const bna::Vector2& axis);

        private:
        Vector2 _center;
        Vector2 _size;
        bn::fixed _rotation; // en grados

        bn::vector<bn::sprite_ptr, 4> _spritesVertices;
        bn::vector<Vector2, 4> _vertices;
        bn::vector<Vector2, 4> _axesNormalized;

        struct ProjectionInfo {
            bool updated = false;
            std::pair<bn::fixed, bn::fixed> projection;
        };
        bn::array<ProjectionInfo, 4> _projectionsInfo;
        bool _axesNormalidedUpdated;

        bn::vector<Vector2, 4> _generateVertices() const;
        bn::vector<Vector2, 4> _generateVertices2() const;

        void _updateSpritesPos();
    };
} // namespace bna
