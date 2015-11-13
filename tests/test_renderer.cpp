#include "renderer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

void error_callback(int error, const char* description)
{
    std::cerr << "error: " << description;
    exit(EXIT_FAILURE);
}

BOOST_AUTO_TEST_CASE(renderer_init) {
    BOOST_CHECK(glfwInit());
    glfwSetErrorCallback(error_callback);
    GLFWwindow* window = glfwCreateWindow(640, 480, "TestWindow", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    BOOST_CHECK_EQUAL(glewInit(), GLEW_OK); // important
    Renderer renderer(640, 680);
    BOOST_CHECK_NO_THROW(renderer.init());
    glfwDestroyWindow(window);
    glfwTerminate();
}
