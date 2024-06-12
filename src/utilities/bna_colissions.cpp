#include "bna_colissions.hpp"
#include "bn_log.h"
namespace bna {
    namespace helper {
        bn::vector<bna::Vector2, 4> getAxes(const bn::vector<bna::Vector2, 4>& vertices) {
            bn::vector<bna::Vector2, 4> axes(vertices.size());
            for (int i = 0; i < vertices.size(); i++) {
                bna::Vector2 p1 = vertices[i];
                bna::Vector2 p2 = vertices[(i + 1) % vertices.size()];
                bna::Vector2 edge = p2 - p1;
                axes[i] = bna::Vector2(-edge.y(), edge.x()); // Normal al borde
            }
            return axes;
        };
        bn::vector<bna::Vector2, 4> getAxesNormalized(const bn::vector<bna::Vector2, 4>& vertices) {
            bn::vector<bna::Vector2, 4> axes(vertices.size());
            for (int i = 0; i < vertices.size(); i++) {
                bna::Vector2 p1 = vertices[i];
                bna::Vector2 p2 = vertices[(i + 1) % vertices.size()];
                bna::Vector2 edge = p2 - p1;
                bna::Vector2 normal = bna::Vector2(-edge.y(), edge.x()).normalize();
                axes[i] = bna::Vector2(normal); // Normal al borde
            }
            return axes;
        };

        std::pair<bn::fixed, bn::fixed> project(const bn::vector<bna::Vector2, 4>& vertices, const bna::Vector2& axis) {
            bn::fixed min = vertices[0].dot(axis);
            bn::fixed max = min;
            for (const bna::Vector2& vertex : vertices) {
                bn::fixed projection = vertex.dot(axis);
                if (projection < min) {
                    min = projection;
                }
                if (projection > max) {
                    max = projection;
                }
            }
            return std::make_pair(min, max);
        };

        auto overlap(const std::pair<bn::fixed, bn::fixed>& proj1, const std::pair<bn::fixed, bn::fixed>& proj2) {
            return !(proj1.second < proj2.first || proj2.second < proj1.first);
        };

        auto getCollisionVertex(const bn::vector<bna::Vector2, 4>& vertices, const bna::Vector2& axis) {
            bn::fixed min = axis.x() * vertices[0].x() + axis.y() * vertices[0].y();
            bna::Vector2 minVertex = vertices[0];
            for (const auto& vertex : vertices) {
                bn::fixed projection = axis.x() * vertex.x() + axis.y() * vertex.y();
                if (projection < min) {
                    min = projection;
                    minVertex = vertex;
                }
            }
            return minVertex;
        };
    }

    CollisionPoint checkCollisionPoint(const bna::Hitbox& hb1, const bna::Hitbox& hb2) {
        bn::vector<bna::Vector2, 4> vertices1 = hb1.getVertices();
        bn::vector<bna::Vector2, 4> vertices2 = hb2.getVertices();

        bn::vector<bna::Vector2, 8> axes = helper::getAxesNormalized(vertices1);
        bn::vector<bna::Vector2, 4> axes2 = helper::getAxesNormalized(vertices2);

        // Convine two vectors in one
        for (int i = 0; i < axes2.size(); i++) {
            axes.push_back(axes2[i]);
        }

        CollisionPoint collisionPoint;
        collisionPoint.collided = false;
        bn::fixed minOverlap;
        Vector2 smallestAxis;
        bool inicializadoMinOverlap = false;

        // BN_LOG("----");
        for (const Vector2& axis : axes) {
            auto [minA, maxA] = helper::project(vertices1, axis);
            auto [minB, maxB] = helper::project(vertices2, axis);

            if (maxA < minB || maxB < minA) {
                return collisionPoint; // No hay colisión
            }
            else {
                bn::fixed overlap = bn::min(maxA, maxB) - bn::max(minA, minB);
                // BN_LOG("Over: ",overlap," Min: ",minOverlap);
                if (!inicializadoMinOverlap) {
                    minOverlap = overlap;
                    smallestAxis = axis;
                    inicializadoMinOverlap = true;
                }
                else if (overlap < minOverlap) {
                    minOverlap = overlap;
                    smallestAxis = axis;
                }
            }
        }

        collisionPoint.collided = true;

        Vector2 direction = hb2.getPosition() - hb1.getPosition();
        if (direction.dot(smallestAxis) < 0) {
            smallestAxis = smallestAxis * -1;
        }

        collisionPoint.correctionVector = smallestAxis * minOverlap;
        collisionPoint.collisionPoint =  hb1.getPosition() - collisionPoint.correctionVector;
        return collisionPoint; // Hay colisión
    }

    bool checkCollision(const bna::Hitbox& hb1, const bna::Hitbox& hb2) {

        bn::vector<bna::Vector2, 4> vertices1 = hb1.getVertices();
        bn::vector<bna::Vector2, 4> vertices2 = hb2.getVertices();

        bn::vector<bna::Vector2, 8> axes = helper::getAxes(vertices1);
        bn::vector<bna::Vector2, 4> axes2 = helper::getAxes(vertices2);

        // Convine two vectors in one
        for (int i = 0; i < axes2.size(); i++) {
            axes.push_back(axes2[i]);
        }

        for (const auto& axis : axes) {
            auto proj1 = helper::project(vertices1, axis);
            auto proj2 = helper::project(vertices2, axis);
            if (!helper::overlap(proj1, proj2)) {
                return false; // Si hay una separación, no hay colisión
            }
        }
        return true; // Si no hay ninguna separación, hay colisión
    }

    CollisionResult checkCollisionFull(const bna::Hitbox& hb1, const bna::Hitbox& hb2) {


        bn::vector<bna::Vector2, 4> vertices1 = hb1.getVertices();
        bn::vector<bna::Vector2, 4> vertices2 = hb2.getVertices();

        bn::vector<bna::Vector2, 8> axes = helper::getAxes(vertices1);
        bn::vector<bna::Vector2, 4> axes2 = helper::getAxes(vertices2);
        for (int i = 0; i < axes2.size(); i++) {
            axes.push_back(axes2[i]);
        }

        for (const auto& axis : axes) {
            auto proj1 = helper::project(vertices1, axis);
            auto proj2 = helper::project(vertices2, axis);
            if (!helper::overlap(proj1, proj2)) {
                return { false, bna::Vector2(), bna::Vector2() }; // Si hay una separación, no hay colisión
            }
        }

        // Si no hay ninguna separación, encontrar los vértices de colisión más cercanos
        bna::Vector2 collisionVertex1 = helper::getCollisionVertex(vertices1, axes[0]);
        bna::Vector2 collisionVertex2 = helper::getCollisionVertex(vertices2, axes[0]);

        return { true, collisionVertex1, collisionVertex2 };
    }
}

