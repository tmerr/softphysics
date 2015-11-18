#include "mainwindow.hpp"

// glad must be included before glfw.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <exception>


class WindowCreationError : public std::runtime_error {
public:
    WindowCreationError(const std::string& msg) 
        : std::runtime_error(msg)
    { }
};

#ifdef DEBUG_GL
void debugCallbackGL(GLenum source, GLenum type, GLuint id, GLenum severity,
                     GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "OpenGL debug message:" << std::endl
              << "    source: " << source << std::endl
              << "    type: " << type << std::endl
              << "    id: " << id << std::endl
              << "    severity: " << severity << std::endl
              << "    message: " << message << std::endl
              << std::endl;

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        throw std::runtime_error("Error: high severity gl error in debug mode");
    }
}
#endif

static void error_callback(int error, const char* description) {
    std::cerr << description;
}

void MainWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void MainWindow::mouseCallbackGlfw(GLFWwindow* window, double xpos, double ypos) {
    MainWindow *win = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));
    if (win->mouse_havelast) {
        glfwGetCursorPos(window, &xpos, &ypos);
        float dx = (float)(xpos - win->mouse_lastx);
        float dy = (float)(ypos - win->mouse_lasty);
        win->mouseCallback(dx, dy);
    }
    win->mouse_lastx = xpos;
    win->mouse_lasty = ypos;
    win->mouse_havelast = true;
}

void MainWindow::resizeCallbackGlfw(GLFWwindow* window, int width, int height) {
    MainWindow *win = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));
    win->resizeCallback(width, height);
}

MainWindow::MainWindow(int width, int height, const char* title) {

    if (!glfwInit()) {
        throw WindowCreationError("glfwInit failed");
    }

    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw WindowCreationError("glfwCreateWindow failed");
    }
    glfwMakeContextCurrent(window);

    // Load GL and check for errors
    if (!gladLoadGL()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw WindowCreationError("gladLoadGL failed");
    }
    std::cout << "OpenGL info:" << std::endl
              << "    OpenGL version: " << glGetString(GL_VERSION) << std::endl
              << "    GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl
              << "    Vendor: " << glGetString(GL_VENDOR) << std::endl
              << "    Renderer: " << glGetString(GL_RENDERER) << std::endl
              << std::endl;
    if (GLVersion.major < 3 || (GLVersion.major == 3 && GLVersion.minor < 3)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw WindowCreationError("Error: expected minimum OpenGL version 3.3");
    }
#ifdef DEBUG_GL
    bool guaranteedDebug = GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 3);
    if (!(guaranteedDebug || GLAD_GL_KHR_debug)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw WindowCreationError("Error: debug callbacks require GL 4.3 or GL_KHR_debug extension");
    }
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallbackGL, NULL);
#endif

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, resizeCallbackGlfw);
    glfwSetCursorPosCallback(window, mouseCallbackGlfw);

    // enables vsync.
    glfwSwapInterval(1);

    // the default resize callback does nothing.
    resizeCallback = [](int w, int h) {};
}

void MainWindow::deltaMouse(float* dx, float* dy) {
}

bool MainWindow::checkLeft() {
    return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
}

bool MainWindow::checkRight() {
    return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
}

bool MainWindow::checkUp() {
    return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
}

bool MainWindow::checkDown() {
    return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
}

void MainWindow::setMouseMoveCallback(std::function<void(float, float)> callback) {
    mouseCallback = callback;
}

void MainWindow::setResizeCallback(std::function<void(int, int)> callback) {
    resizeCallback = callback;
}

void MainWindow::pollEvents() {
    glfwPollEvents();
}

void MainWindow::swapBuffers() {
    glfwSwapBuffers(window);
}

bool MainWindow::shouldClose() {
    return glfwWindowShouldClose(window);
}

MainWindow::~MainWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
