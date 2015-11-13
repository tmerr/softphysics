#include "renderer.hpp"
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <exception>

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/mat4x4.hpp>

class ShaderError : public std::exception
{
  virtual const char* what() const throw()
  {
    return "error setting up the shaders";
  }
} shader_error;

GLuint compileShader(std::string path, GLenum shader_type) {
    const char* src;
    try {
        std::ifstream t(path);
        std::string ts((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        src = ts.c_str();
    } catch(std::ifstream::failure e) {
        std::cerr << e.what() << std::endl;
        throw shader_error;
    }

    GLuint shader = glCreateShader(shader_type);
    glShaderSourceARB(shader, 1, &src, NULL);
    glCompileShaderARB(shader);

    GLint is_compiled;
    glGetProgramiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
        GLint loglength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglength);
        std::vector<GLchar> log(loglength);
        glGetShaderInfoLog(shader, loglength, &loglength, &log[0]);
        std::cerr << "failed to compile shader at " << path << "." << std::endl;
        for (auto &ch : log) {
            std::cerr << ch;
        }
        std::cerr << std::endl;

        glDeleteShader(shader);
        throw shader_error;
    }

    return shader;
}

GLuint compileSolidShader() {
    GLuint vert = compileShader("shaders/solid.vert", GL_VERTEX_SHADER);
    GLuint frag;
    try {
        frag = compileShader("shaders/solid.frag", GL_FRAGMENT_SHADER);
    } catch(ShaderError e) {
        glDeleteShader(vert);
        throw;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    GLint is_linked;
    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
    if(is_linked == GL_FALSE)
    {
        GLint loglength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &loglength);
        std::vector<GLchar> log(loglength);
        glGetProgramInfoLog(program, loglength, &loglength, &log[0]);
        std::cerr << "failed to link shader program for solid material." << std::endl;
        for (auto &ch : log) {
            std::cerr << ch;
        }
        std::cerr << std::endl;
        // don't leak
        glDeleteProgram(program);
        glDeleteShader(vert);
        glDeleteShader(frag);
        throw shader_error;
    }

    glDetachShader(program, vert);
    glDetachShader(program, frag);

    return program;
}

void Renderer::init() {
    solid_program = compileSolidShader();
}

void Renderer::windowChanged(int width, int height) {
    glViewport(0.f, 0.f, (float)width, (float)height);
}

void Renderer::render(const std::vector<SimObject> &objects, glm::mat4 worldtoclip) {
}

Renderer::~Renderer() {
    // solid_program may be 0, that's ok.
    glDeleteProgram(solid_program);
}
