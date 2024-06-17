#include "bna_colissions.hpp"
#include "bn_log.h"
#include "bn_profiler.h"
#include "bn_math.h"

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

    CollisionPoint checkCollisionPointV2(const bna::Hitbox& hb1, const bna::Hitbox& hb2) {
        bn::vector<bna::Vector2, 4> vertices1 = hb1.getVertices();
        bn::vector<bna::Vector2, 4> vertices2 = hb2.getVertices();

        CollisionPoint collisionPoint;
        collisionPoint.collided = false;

        bn::fixed overlap;
        bool overlap_inicializado = false;
        // BN_LOG("------");
        BN_PROFILER_START("ColPointV2");
        for (int shape = 0; shape < 2; shape++) {
            if (shape == 1) {
                vertices1 = hb2.getVertices();
                vertices2 = hb1.getVertices();
            }
            for (int a = 0; a < vertices1.size(); a++) {
                int b = (a + 1) % vertices1.size();
                bna::Vector2 axisProj = { -(vertices1[b].y() - vertices1[a].y()), vertices1[b].x() - vertices1[a].x() };

                // Optional normalisation of projection axis enhances stability slightly
                //bn::fixed d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
                //axisProj = { axisProj.x / d, axisProj.y / d };

                // Work out min and max 1D points for r1
                bn::fixed min_r1;
                bn::fixed max_r1;
                bool r1_inicializado = false;
                for (int p = 0; p < vertices1.size(); p++) {
                    bn::fixed q = (vertices1[p].x() * axisProj.x() + vertices1[p].y() * axisProj.y());
                    if (!r1_inicializado) {
                        min_r1 = min_r1;
                        max_r1 = max_r1;
                        r1_inicializado = true;
                    }
                    else {
                        min_r1 = bn::min(min_r1, q);
                        max_r1 = bn::max(max_r1, q);
                    }
                }

                // Work out min and max 1D points for r2
                bn::fixed min_r2;
                bn::fixed max_r2;
                bool r2_inicializado = false;
                for (int p = 0; p < vertices2.size(); p++) {
                    bn::fixed q = (vertices2[p].x() * axisProj.x() + vertices2[p].y() * axisProj.y());
                    if (!r2_inicializado) {
                        min_r2 = bn::min(min_r2, q);
                        max_r2 = bn::max(max_r2, q);
                        r2_inicializado = true;
                    }
                    else {
                        min_r2 = bn::min(min_r2, q);
                        max_r2 = bn::max(max_r2, q);
                    }
                }
                // BN_LOG(overlap);
                // Calculate actual overlap along projected axis, and store the minimum
                if (!overlap_inicializado) {
                    overlap = bn::min(max_r1, max_r2) - bn::max(min_r1, min_r2);
                    overlap_inicializado = true;
                }
                else {
                    overlap = bn::min(bn::min(max_r1, max_r2) - bn::max(min_r1, min_r2), overlap);
                }

                if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
                    return collisionPoint;
            }
        }
        BN_PROFILER_STOP();

        // If we got here, the objects have collided, we will displace r1
        // by overlap along the vector between the two object centers
        collisionPoint.collided = true;

        bna::Vector2 d = { hb2.getPosition().x() - hb1.getPosition().x(), hb2.getPosition().y() - hb1.getPosition().y() };
        bn::fixed s = sqrt(d.x() * d.x() + d.y() * d.y());
        collisionPoint.correctionVector = d * -overlap / s;
        // hb1.pos.x -= overlap * d.x() / s;
        // hb1.pos.y -= overlap * d.y() / s;
        return collisionPoint;
    }

    CollisionPoint checkCollisionPoint(const bna::Hitbox& hb1, const bna::Hitbox& hb2) {
        bn::vector<bna::Vector2, 4> vertices1 = hb1.getVertices();
        bn::vector<bna::Vector2, 4> vertices2 = hb2.getVertices();

        BN_PROFILER_START("ColPoint GetAxesNormalized");
        bn::vector<bna::Vector2, 8> axes = helper::getAxesNormalized(vertices1);
        bn::vector<bna::Vector2, 4> axes2 = helper::getAxesNormalized(vertices2);
        BN_PROFILER_STOP();
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
        BN_PROFILER_START("ColPoint Checkoverlap");
        for (const Vector2& axis : axes) {
            auto [minA, maxA] = helper::project(vertices1, axis);
            auto [minB, maxB] = helper::project(vertices2, axis);

            if (maxA < minB || maxB < minA) {
                BN_PROFILER_STOP();
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
        BN_PROFILER_STOP();

        collisionPoint.collided = true;

        BN_PROFILER_START("ColPoint Check overlap");
        Vector2 direction = hb2.getPosition() - hb1.getPosition();
        if (direction.dot(smallestAxis) < 0) {
            smallestAxis = smallestAxis * -1;
        }

        collisionPoint.correctionVector = smallestAxis * minOverlap;
        collisionPoint.collisionPoint = hb1.getPosition() + collisionPoint.correctionVector;
        BN_PROFILER_STOP();
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

