#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include "scene.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class MainWindow {
public:
    MainWindow(int width, int height, const char* title);
    void mainLoop();
private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void resizeCallback(GLFWwindow* window, int width, int height);
    Scene scene;
    GLFWwindow* window;
};

#endif
