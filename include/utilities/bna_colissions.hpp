#pragma once

#include "bna_hitbox.hpp"
#include "bn_vector.h"

namespace bna {
    namespace helper {
        bn::vector<bna::Vector2, 4> getAxes(const bn::vector<bna::Vector2, 4>& vertices);
        bn::vector<bna::Vector2, 4> getAxesNormalized(const bn::vector<bna::Vector2, 4>& vertices);

        bn::pair<bn::fixed, bn::fixed> project(const bn::vector<bna::Vector2, 4>& vertices, const bna::Vector2& axis);

        auto overlap(const bn::pair<bn::fixed, bn::fixed>& proj1, const bn::pair<bn::fixed, bn::fixed>& proj2);

        auto getCollisionVertex(const bn::vector<bna::Vector2, 4>& vertices, const bna::Vector2& axis);
    }

    bool checkCollision(bna::Hitbox& hb1, bna::Hitbox& hb2);

    struct CollisionPoint {
        bool collided = false;
        Vector2 collisionPoint;
        Vector2 correctionVector;
    };
    CollisionPoint checkCollisionPoint(const bna::Hitbox& hb1, const bna::Hitbox& hb2);
    CollisionPoint checkCollisionPointV2(bna::Hitbox& hb1, bna::Hitbox& hb2);

    struct CollisionResult {
        bool collided;
        bna::Vector2 collisionVertex1;
        bna::Vector2 collisionVertex2;
    };

    CollisionResult checkCollisionFull(const bna::Hitbox& hb1, const bna::Hitbox& hb2);
}
