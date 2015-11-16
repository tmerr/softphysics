#include "boundingbox.hpp"
#include <glm/vec3.hpp>
#include <algorithm>
#include <limits>
#include <vector>

BoundingBox BoundingBox::ofPoints(const std::vector<glm::vec3> &points) {
    static const float inf = std::numeric_limits<float>::infinity();
    BoundingBox bb;
    bb.p0.x = inf;
    bb.p0.y = inf;
    bb.p0.z = inf;
    bb.p1.x = -inf;
    bb.p1.y = -inf;
    bb.p1.z = -inf;
    for (auto &p : points) {
        bb.p0.x = std::min(bb.p0.x, p.x);
        bb.p0.y = std::min(bb.p0.y, p.y);
        bb.p0.z = std::min(bb.p0.z, p.z);
        bb.p1.x = std::max(bb.p1.x, p.x);
        bb.p1.y = std::max(bb.p1.y, p.y);
        bb.p1.z = std::max(bb.p1.z, p.z);
    }
    return bb;
}

bool BoundingBox::contains(const glm::vec3 p) {
    return p0.x <= p.x && p.x < p1.x &&
           p0.y <= p.y && p.y < p1.y &&
           p0.z <= p.z && p.z < p1.z;
}
