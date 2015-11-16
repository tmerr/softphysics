#ifndef BOUNDINGBOX_HPP_INCLUDED
#define BOUNDINGBOX_HPP_INCLUDED

#include <glm/vec3.hpp>
#include <vector>

struct BoundingBox {
    glm::vec3 p0;
    glm::vec3 p1;

    static BoundingBox ofPoints(const std::vector<glm::vec3> &points);
    bool contains(const glm::vec3 p);
};

#endif
