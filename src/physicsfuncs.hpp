#ifndef PHYSICSFUNCS_HPP_INCLUDED
#define PHYSICSFUNCS_HPP_INCLUDED

#include "simobject.hpp"
#include <boost/variant.hpp>
#include <vector>
#include <cstddef>
#include <glm/vec3.hpp>


namespace physicsfuncs {

bool rayIntersectsTriangle(glm::vec3 p, glm::vec3 d,
                           glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
                           float* u, float* v);

typedef size_t PointPenetration;

// gets which face in object2 was penetrated.
//
// the location point of the face penetration is at
// u*v1 + v*v2 + w*v3
// where u + v + w = 1
// (so you have the info to calculate w)
struct FacePenetration {
    size_t which;

    float u;
    float v;
};

typedef boost::variant<PointPenetration, FacePenetration> Penetration;

bool broadphase(const SimObject & object1,
                const SimObject & object2);

std::vector<size_t> narrowphase(const SimObject & object1,
                                const SimObject & object2);

std::vector<Penetration> calcPenetration(const SimObject & object1,
                                         const SimObject & object2,
                                         std::vector<size_t> & pointsInside);

std::vector<glm::vec3> mutateForces(const SimObject & object1,
                                    const SimObject & object2,
                                    std::vector<Penetration> penetrations);

}

#endif
