#ifndef PHYSICSFUNCS_HPP_INCLUDED
#define PHYSICSFUNCS_HPP_INCLUDED

#include "simobject.hpp"
#include <boost/variant.hpp>
#include <vector>
#include <cstddef>


namespace physicsfuncs {

typedef size_t PointPenetration;

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
