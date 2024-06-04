#pragma once

#include "bna_hitbox.hpp"
#include "bn_vector.h"

bool checkCollision(const bna::Hitbox& hb1, const bna::Hitbox& hb2) {
    auto getAxes = [](const bn::vector<bna::Vector2, 4>& vertices) {
        bn::vector<bna::Vector2, 4> axes(vertices.size());
        for (int i = 0; i < vertices.size(); i++) {
            bna::Vector2 p1 = vertices[i];
            bna::Vector2 p2 = vertices[(i + 1) % vertices.size()];
            bna::Vector2 edge = p2 - p1;
            axes[i] = bna::Vector2(-edge.y(), edge.x()); // Normal al borde
        }
        return axes;
        };

    auto project = [](const bn::vector<bna::Vector2,4>& vertices, const bna::Vector2& axis) {
        bn::fixed min = axis.x() * vertices[0].x() + axis.y() * vertices[0].y();
        bn::fixed max = min;
        for (const auto& vertex : vertices) {
            bn::fixed projection = axis.x() * vertex.x() + axis.y() * vertex.y();
            if (projection < min) min = projection;
            if (projection > max) max = projection;
        }
        return std::make_pair(min, max);
        };

    auto overlap = [](const std::pair<bn::fixed, bn::fixed>& proj1, const std::pair<bn::fixed, bn::fixed>& proj2) {
        return !(proj1.second < proj2.first || proj2.second < proj1.first);
        };

    bn::vector<bna::Vector2, 4> vertices1 = hb1.getVertices();
    bn::vector<bna::Vector2, 4> vertices2 = hb2.getVertices();

    bn::vector<bna::Vector2, 8> axes = getAxes(vertices1);
    bn::vector<bna::Vector2, 4> axes2 = getAxes(vertices2);

    // Convine two vectors in one
    for (int i = 0; i < axes2.size(); i++) {
       axes.push_back(axes2[i]);
    }

    for (const auto& axis : axes) {
        auto proj1 = project(vertices1, axis);
        auto proj2 = project(vertices2, axis);
        if (!overlap(proj1, proj2)) {
            return false; // Si hay una separación, no hay colisión
        }
    }
    return true; // Si no hay ninguna separación, hay colisión
}
