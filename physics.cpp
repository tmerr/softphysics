#include <glm/vec3.hpp>
#include <cstddef>
#include <array>

// applied to a point on the body
struct PointForce {
    std::size_t point;
    glm::vec3 force;
};

// applied to all points on the body within the volume
struct CylinderForce {
    glm::vec3 bottom;
    glm::vec3 totop;
    float radius;
};


