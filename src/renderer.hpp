#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include "simobject.hpp"
#include <glm/mat4x4.hpp>
#include <vector>
#include <glad/glad.h>

class Renderer {
public:
    ~Renderer();
    void init(int width, int height);
    void windowChanged(int width, int height);
    void render(const std::vector<SimObject> &objects, glm::mat4 worldtoclip);
private:
    GLuint solid_program;
};

#endif
