#include "renderer.hpp"
#include "simobject.hpp"

#include <glm/mat4x4.hpp>

#include <boost/variant.hpp>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <exception>

class ShaderError : public std::exception {
  virtual const char* what() const throw() {
    return "error setting up the shaders";
  }
} shader_error;

GLuint compileShader(std::string path, GLenum shader_type) {
    // read in the shader source code
    std::string source_string;
    try {
        std::ifstream file(path);
        source_string = std::string((std::istreambuf_iterator<char>(file)),
                                     std::istreambuf_iterator<char>());
    } catch(std::ifstream::failure e) {
        std::cerr << e.what() << std::endl;
        throw shader_error;
    }

    // create and compile the shader
    const char* source = source_string.c_str();
    int length = source_string.size();
    GLuint shader = glCreateShader(shader_type);
    if (shader == 0) {
        std::cerr << "failed to create shader";
        throw shader_error;
    }
    glShaderSource(shader, 1, &source, &length);
    glCompileShader(shader);

    // check the compile status
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint loglength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglength);

        std::cerr << "failed to compile shader at \"" << path << "\"." << std::endl;

        if (loglength > 1) {
            std::vector<GLchar> log(loglength);
            glGetShaderInfoLog(shader, loglength, &loglength, &log[0]);
            std::cerr << "info log: " << &log[0] << std::endl;
        }

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
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
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
        glDeleteProgram(program);
        glDeleteShader(vert);
        glDeleteShader(frag);
        throw shader_error;
    }

    glDetachShader(program, vert);
    glDetachShader(program, frag);

    return program;
}


void Renderer::init(int width, int height) {
    glViewport(0.f, 0.f, (float)width, (float)height);
    solid_program = compileSolidShader();
}

void Renderer::windowChanged(int width, int height) {
    glViewport(0.f, 0.f, (float)width, (float)height);
}

struct DrawData {
    const std::vector<glm::vec3>& points;
    const std::vector<std::array<unsigned int, 3> >& faces;
};

class DataGrabber: public boost::static_visitor<DrawData> {
public:
    DrawData operator()(const StaticBody &st) const {
        return DrawData { st.points, st.faces };
    }
    DrawData operator()(const SoftBody &so) const {
        return DrawData { so.points, so.faces };
    }
};

class DrawIfSolid: public boost::static_visitor<> {
public:
    DrawIfSolid(DrawData data) : data(data) { }
    void operator()(const SolidMaterial &solid) const {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        GLsizeiptr vbo_sz = data.points.size() * sizeof(glm::vec3);
        glBufferData(GL_ARRAY_BUFFER, vbo_sz, &data.points[0], GL_STREAM_DRAW);

        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        GLsizeiptr ibo_sz = data.faces.size() * sizeof(unsigned int) * 3;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_sz, &data.faces[0], GL_STREAM_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawElements(GL_TRIANGLES, data.faces.size(), GL_UNSIGNED_INT, 0);

        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
    template<typename ANY>
    void operator()(const ANY &so) const { }
private:
    DrawData data;
};

void Renderer::render(const std::vector<SimObject> &objects, glm::mat4 worldtoclip) {
    glClearColor(0.2, 0.2, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // use the solid program
    glUseProgram(solid_program);

    // set the uniforms
    GLint location = glGetUniformLocation(solid_program, "worldtoclip");
    if (location == -1) {
        std::cerr << "couldn't find uniform variable worldtoclip" << std::endl;
        throw shader_error;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, &worldtoclip[0][0]);

    // make + draw buffers
    for (const SimObject &obj : objects) {
        DrawData draw_data = boost::apply_visitor(DataGrabber(), obj.body);
        DrawIfSolid drawIfSolid(draw_data);
        boost::apply_visitor(drawIfSolid, obj.material);
    }
}

Renderer::~Renderer() {
    // solid_program may be 0, that's ok.
    glDeleteProgram(solid_program);
}
