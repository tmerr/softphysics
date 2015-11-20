#include "renderer.hpp"
#include <boost/test/included/unit_test.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void error_callback(int error, const char* description)
{
    std::cerr << "error: " << description;
    exit(EXIT_FAILURE);
}

BOOST_AUTO_TEST_CASE(renderer_init) {
    BOOST_CHECK(glfwInit());
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "TestWindow", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    BOOST_CHECK(gladLoadGL());
    Renderer renderer(640, 480);
    glfwDestroyWindow(window);
    glfwTerminate();
}
