#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include "simobject.hpp"
#include <GL/glew.h>
#include <vector>

class Renderer {
public:
    void initialize();

    // render the scene
    void render(const std::vector<SimObject> &objects); // , Camera camera) <- will need this
private:
    GLuint solid_program;
};

#endif
