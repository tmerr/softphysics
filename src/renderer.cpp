#include "renderer.hpp"
#include "simobject.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <boost/variant.hpp>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <exception>

class ShaderError : public std::runtime_error {
public:
  ShaderError(const std::string& msg)
      : std::runtime_error(msg)
  {}
};

GLuint compileShader(std::string path, GLenum shader_type) {
    // read in the shader source code
    std::string source_string;
    try {
        std::ifstream file(path);
        source_string = std::string((std::istreambuf_iterator<char>(file)),
                                     std::istreambuf_iterator<char>());
    } catch(std::ifstream::failure e) {
        throw ShaderError("error with \"" + path + "\". couldn't read file. exception: " + e.what());
    }

    // create and compile the shader
    const char* source = source_string.c_str();
    int length = source_string.size();
    GLuint shader = glCreateShader(shader_type);
    if (shader == 0) {
        throw ShaderError("error with \"" + path + "\". glCreateShader returned 0.");
    }
    glShaderSource(shader, 1, &source, &length);
    glCompileShader(shader);

    // check the compile status
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint loglength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglength);

        std::stringstream errmsg;
        errmsg << "error with \"" << path << "\". gl could not compile the shader. ";

        if (loglength > 1) {
            std::vector<GLchar> log(loglength);
            glGetShaderInfoLog(shader, loglength, &loglength, &log[0]);
            errmsg << "info log: " << &log[0];
        } else {
            errmsg << "there is no associated log.";
        }

        glDeleteShader(shader);
        throw ShaderError(errmsg.str());
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
        std::stringstream errmsg;
        errmsg << "failed to link shader program for solid material. ";

        GLint loglength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &loglength);
        if (loglength > 1) {
            std::vector<GLchar> log(loglength);
            glGetProgramInfoLog(program, loglength, &loglength, &log[0]);
            errmsg << "info log: " << &log[0];
        } else {
            errmsg << "there is no associated log.";
        }
        glDeleteProgram(program);
        glDeleteShader(vert);
        glDeleteShader(frag);
        throw ShaderError(errmsg.str());
    }

    glDetachShader(program, vert);
    glDetachShader(program, frag);

    return program;
}

Renderer::Renderer(int viewportWidth, int viewportHeight) {
    glViewport(0.f, 0.f, (float)viewportWidth, (float)viewportHeight);
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
    DrawIfSolid(DrawData data, GLint col_uniform) : data(data), color_uniform(col_uniform) { }
    void operator()(const SolidMaterial &solid) const {
        glUniform3fv(color_uniform, 1, glm::value_ptr(solid.color));

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        GLsizeiptr vbo_sz = data.points.size() * sizeof(glm::vec3);
        glBufferData(GL_ARRAY_BUFFER, vbo_sz, glm::value_ptr(data.points[0]), GL_STREAM_DRAW);

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        GLsizeiptr ibo_sz = data.faces.size() * sizeof(unsigned int) * 3;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_sz, &data.faces[0][0], GL_STREAM_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawElements(GL_TRIANGLES, data.faces.size() * 3, GL_UNSIGNED_INT, 0);

        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &vao);
    }
    template<typename ANY>
    void operator()(const ANY &so) const { }
private:
    DrawData data;
    GLint color_uniform;
};

void Renderer::render(const std::vector<SimObject> &objects, glm::mat4 worldtoclip) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the solid program
    glUseProgram(solid_program);

    // set the world to clip matrix uniform
    GLint matrix_uniform = glGetUniformLocation(solid_program, "worldtoclip");
    if (matrix_uniform == -1) {
        throw ShaderError("couldn't get uniform variable worldtoclip");
    }
    glUniformMatrix4fv(matrix_uniform, 1, GL_FALSE, glm::value_ptr(worldtoclip));

    // get the color uniform that will be changed with each simulation object
    GLint color_uniform = glGetUniformLocation(solid_program, "input_color");
    if (color_uniform == -1) {
        throw ShaderError("couldn't get uniform variable input_color");
    }

    // make + draw buffers
    for (const SimObject &obj : objects) {
        DrawData draw_data = boost::apply_visitor(DataGrabber(), obj.body);
        DrawIfSolid drawIfSolid(draw_data, color_uniform);
        boost::apply_visitor(drawIfSolid, obj.material);
    }
}

Renderer::~Renderer() {
    // solid_program may be 0, that's ok.
    glDeleteProgram(solid_program);
}
