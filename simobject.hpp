#ifndef SIMOBJECT_HPP_INCLUDED
#define SIMOBJECT_HPP_INCLUDED

#include "boundingbox.hpp"
#include <boost/variant.hpp>
#include <glm/vec3.hpp>
#include <array>
#include <cstddef>
#include <vector>

// bodies

struct StaticBody {
    std::vector<glm::vec3> points;
    std::vector<std::array<std::size_t, 3> > faces;
    BoundingBox boundingbox;
};

struct Spring {
    std::size_t a;
    std::size_t b;
    float k;
    float restlength;
};

struct SoftBody {
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> points_backbuffer;
    std::vector<glm::vec3> velocities;
    std::vector<glm::vec3> velocities_backbuffer;
    std::vector<std::array<std::size_t, 3> > faces;
    std::vector<Spring> springs;
};

// materials

struct SolidMaterial {
    glm::vec3 color;
};

struct JelloMaterial {
    glm::vec3 color;
    float transparency; // 0 to 1
};

// simulation object

struct SimObject {
    boost::variant<StaticBody, SoftBody> body;
    boost::variant<SolidMaterial, JelloMaterial> material;
};

#endif
