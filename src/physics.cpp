#include "physics.hpp"
#include <glm/vec3.hpp>
#include <cstddef>
#include <vector>

// applied to a point on the body
struct PointForce {
    std::size_t point;
    glm::vec3 force;
};

// applied to all points on the body within the volume
struct CylinderForce {
    glm::vec3 start;
    glm::vec3 end;
    float radius;
};

void Physics::step(std::vector<SimObject> &simobjects, float dt) {
}