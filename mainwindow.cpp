#include "mainwindow.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>

class WindowCreationError : public std::runtime_error {
public:
    WindowCreationError(const std::string& msg) 
        : std::runtime_error(msg)
    { }
};

static void error_callback(int error, const char* description) {
    std::cerr << description;
}

void MainWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    MainWindow *win = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));

    switch (key) {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            break;
        case GLFW_KEY_W:
            if (action == GLFW_PRESS) {
                win->scene.forwardPressed();
            } else if (action == GLFW_RELEASE) {
                win->scene.forwardReleased();
            }
            break;
        case GLFW_KEY_A:
            if (action == GLFW_PRESS) {
                win->scene.leftPressed();
            } else if (action == GLFW_RELEASE) {
                win->scene.leftReleased();
            }
            break;
        case GLFW_KEY_S:
            if (action == GLFW_PRESS) {
                win->scene.backPressed();
            } else if (action == GLFW_RELEASE) {
                win->scene.backReleased();
            }
            break;
        case GLFW_KEY_D:
            if (action == GLFW_PRESS) {
                win->scene.rightPressed();
            } else if (action == GLFW_RELEASE) {
                win->scene.rightReleased();
            }
            break;
    }
}

void MainWindow::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    MainWindow *win = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));
    win->scene.mouseMoved((float)xpos, (float)ypos);
}

void MainWindow::resizeCallback(GLFWwindow* window, int width, int height) {
    MainWindow *win = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));
    win->scene.windowChanged(width, height);
}

MainWindow::MainWindow(int width, int height, const char* title) {

    if (!glfwInit()) {
        throw WindowCreationError("glfwInit failed");
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw WindowCreationError("glfwCreateWindow failed");
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw WindowCreationError("glewInit failed");
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    glfwSwapInterval(1); // enable vsync
}

void MainWindow::mainLoop() {
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);
    scene.init(width, height);

    while (!glfwWindowShouldClose(window)) {
        //scene.fixedStep();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
