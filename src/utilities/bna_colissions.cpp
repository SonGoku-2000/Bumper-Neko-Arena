#include "bna_colissions.hpp"
#include "bn_math.h"
#include "bn_memory.h"

#ifdef DEBUG
#include "bn_log.h"
#endif
// #define PROFILE
#ifdef PROFILE
#include "bn_profiler.h"
#endif

namespace bna {
    namespace helper {
        bn::vector<bna::Vector2, 4> getAxes(const bn::vector<bna::Vector2, 4>& vertices) {
            bn::vector<bna::Vector2, 4> axes(vertices.size());
            for (int i = 0; i < vertices.size(); i++) {
                bna::Vector2 p1 = vertices[i];
                bna::Vector2 p2 = vertices[(i + 1) % vertices.size()];
                bna::Vector2 edge = p2 - p1;
                bna::Vector2 normal(-edge.y(), edge.x());
                axes[i] = bna::Vector2(-edge.y(), edge.x()); // Normal al borde
            }
            return axes;
        };
        bn::vector<bna::Vector2, 4> getAxesNormalized(const bn::vector<bna::Vector2, 4>& vertices) {
#ifdef PROFILE
            BN_PROFILER_START("CollGetAxisNorm");
#endif
            bn::vector<bna::Vector2, 4> axes(vertices.size());
            for (int i = 0; i < vertices.size(); i++) {
                bna::Vector2 p1 = vertices[i];
                bna::Vector2 p2 = vertices[(i + 1) % vertices.size()];
                bna::Vector2 edge = p2 - p1;
                bna::Vector2 normal = bna::Vector2(-edge.y(), edge.x()).normalize();
                axes[i] = bna::Vector2(normal); // Normal al borde
            }
#ifdef PROFILE
            BN_PROFILER_STOP();
#endif
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
                else if (projection > max) {
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

    CollisionPoint checkCollisionPointV2(bna::Hitbox& hb1, bna::Hitbox& hb2) {
        // const bn::vector<bna::Vector2, 4>& vertices1 = hb1.getVertices();
        // const bn::vector<bna::Vector2, 4>& vertices2 = hb2.getVertices();
#ifdef PROFILE
        BN_PROFILER_START("Coll2 Get axes normal");
#endif
        bn::vector<bna::Vector2, 4> axes1 = hb1.getAxesNormalized();
        bn::vector<bna::Vector2, 4> axes2 = hb2.getAxesNormalized();
#ifdef PROFILE

        BN_PROFILER_STOP();

        BN_PROFILER_START("Coll2 Axes Copy");
        // for (int i = 0; i < axes2.size(); i++) {
        //     axes.push_back(axes2[i]);
        // }
        BN_PROFILER_STOP();
#endif
        CollisionPoint collisionPoint;
        collisionPoint.collided = false;
        bn::fixed minOverlap;
        Vector2 smallestAxis;
        bool inicializadoMinOverlap = false;

#ifdef PROFILE
        BN_PROFILER_START("Collv2 project");
#endif
        for (int i = 0; i < 4; i++) {
            const Vector2& axis = axes1[i];
            auto [minA, maxA] = hb1.getProjectionNormalized(i);
            auto [minB, maxB] = hb2.getProjection(axis);
            // auto [minA, maxA] = helper::project(vertices1, axis);
            // auto [minB, maxB] = helper::project(vertices2, axis);
            if (maxA < minB || maxB < minA) {
#ifdef PROFILE
                BN_PROFILER_STOP();
#endif
                return collisionPoint; // No hay colisión
            }
            else {
                bn::fixed overlap = bn::min(maxA, maxB) - bn::max(minA, minB);
                if (!inicializadoMinOverlap || overlap < minOverlap) {
                    minOverlap = overlap;
                    smallestAxis = axis;
                    inicializadoMinOverlap = true;
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            const Vector2& axis = axes2[i];
            auto [minA, maxA] = hb1.getProjection(axis);
            auto [minB, maxB] = hb2.getProjectionNormalized(i);
            // auto [minA, maxA] = helper::project(vertices1, axis);
            // auto [minB, maxB] = helper::project(vertices2, axis);
            if (maxA < minB || maxB < minA) {

#ifdef PROFILE
                BN_PROFILER_STOP();
#endif
                return collisionPoint; // No hay colisión
            }
            else {
                bn::fixed overlap = bn::min(maxA, maxB) - bn::max(minA, minB);
                if (!inicializadoMinOverlap || overlap < minOverlap) {
                    minOverlap = overlap;
                    smallestAxis = axis;
                    inicializadoMinOverlap = true;
                }
            }
        }
#ifdef PROFILE
        BN_PROFILER_STOP();
#endif
        collisionPoint.collided = true;

        Vector2 direction = hb2.getPosition() - hb1.getPosition();
        if (direction.dot(smallestAxis) < 0) {
            smallestAxis = smallestAxis * -1;
        }

        collisionPoint.correctionVector = smallestAxis * minOverlap;
        collisionPoint.collisionPoint = hb1.getPosition() + collisionPoint.correctionVector;

        return collisionPoint; // Hay colisión
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

#ifdef PROFILE
        BN_PROFILER_START("ColPoint Check overlap");
#endif
        Vector2 direction = hb2.getPosition() - hb1.getPosition();
        if (direction.dot(smallestAxis) < 0) {
            smallestAxis = smallestAxis * -1;
        }

        collisionPoint.correctionVector = smallestAxis * minOverlap;
        collisionPoint.collisionPoint = hb1.getPosition() + collisionPoint.correctionVector;

#ifdef PROFILE
        BN_PROFILER_STOP();
#endif
        return collisionPoint; // Hay colisión
    }



    bool checkCollision(bna::Hitbox& hb1, bna::Hitbox& hb2) {

        // bn::vector<bna::Vector2, 4> vertices1 = hb1.getVertices();
        // bn::vector<bna::Vector2, 4> vertices2 = hb2.getVertices();

#ifdef PROFILE
        BN_PROFILER_START("CheColl Axes");
#endif
        bn::vector<bna::Vector2, 4> axes1 = hb1.getAxes();
        bn::vector<bna::Vector2, 4> axes2 = hb2.getAxes();
#ifdef PROFILE
        BN_PROFILER_STOP();
#endif

        // Convine two vectors in one
#ifdef PROFILE
        BN_PROFILER_START("CheColl Axes Copy");
        BN_PROFILER_STOP();

        BN_PROFILER_START("cheColl Proj");
#endif
        for (int i = 0; i < 4; i++) {
            const Vector2& axis = axes1[i];
            auto proj1 = hb1.getProjection(i);
            auto proj2 = hb2.getProjection(axis);
            if (!helper::overlap(proj1, proj2)) {
#ifdef PROFILE
                BN_PROFILER_STOP();
#endif
                return false; // Si hay una separación, no hay colisión
            }
        }

        for (int i = 0; i < 4; i++) {
            const Vector2& axis = axes2[i];
            auto proj1 = hb1.getProjection(axis);
            auto proj2 = hb2.getProjection(i);
            if (!helper::overlap(proj1, proj2)) {
#ifdef PROFILE
                BN_PROFILER_STOP();
#endif
                return false; // Si hay una separación, no hay colisión
            }
        }

#ifdef PROFILE
        BN_PROFILER_STOP();
#endif

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

