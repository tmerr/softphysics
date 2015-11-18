#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <functional>

// Let's hold off on the glfw #include to keep this header more self-contained.
struct GLFWwindow;

// Upon construction, a window guarantees 
// 1. An OpenGL 3.3+ context.
// 2. Debugging capabilities when DEBUG_GL is defined.
//
// If these cannot be guaranteed the window must throw an exception.
//
// Upon destruction, the window and context are destroyed.
class MainWindow {
public:
    MainWindow(int width, int height, const char* title);

    // the change in mouse position is relative to the previous pollEvents call.
    void deltaMouse(float* dx, float* dy);

    // self explanitory.
    bool checkLeft();
    bool checkRight();
    bool checkUp();
    bool checkDown();

    // set the callback for when the mouse moves. the change in position is
    // relative to the last pollEvents call.
    void setMouseMoveCallback(std::function<void(float, float)> callback);

    // set the callback for when the window size changes. the default callback
    // does nothing.
    void setResizeCallback(std::function<void(int, int)> callback);

    // updates the input and triggers event callbacks.
    void pollEvents();

    // swaps the front and back buffers.
    void swapBuffers();

    // becomes true when the window should close.
    bool shouldClose();

    ~MainWindow();

private:
    GLFWwindow* window;
    bool mouse_havelast = false;
    double mouse_lastx;
    double mouse_lasty;
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallbackGlfw(GLFWwindow* window, double xpos, double ypos);
    static void resizeCallbackGlfw(GLFWwindow* window, int width, int height);
    std::function<void(int, int)> resizeCallback;
    std::function<void(float, float)> mouseCallback;
};

#endif
