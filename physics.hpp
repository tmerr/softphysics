#ifndef PHYSICS_HPP_INCLUDED
#define PHYSICS_HPP_INCLUDED

#include "simobject.hpp"
#include <vector>

class Physics {
public:
    void step(std::vector<SimObject> &simobjects, float dt);
};

#endif
