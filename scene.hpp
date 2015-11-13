#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include "camera.hpp"
#include "physics.hpp"
#include "renderer.hpp"
#include <vector>

// The scene in principle doesn't care whether we're using glfw / glut. It requires
// 1. There is a current gl context v3.2 (with loaded extensions).
// 2. It is told about input events.
class Scene {
public:
    Scene(int width, int height);

    // when the forward key is pressed.
    void forwardPressed();

    // when the forward key is released.
    void forwardReleased();

    // when the back key is pressed.
    void backPressed();

    // when the back key is released.
    void backReleased();

    // when the left key is pressed.
    void leftPressed();

    // when the left key is released.
    void leftReleased();

    // when the right key is pressed.
    void rightPressed();

    // when the right key is released.
    void rightReleased();

    // when the mouse moved.
    void mouseMoved(float dx, float dy);

    // when there's a change in window size.
    void windowChanged(int width, int height);

    // take a step in the simulation and rendering.
    void step(float dt);

    // take a step in the simulation and rendering, with the fixed step size as dt.
    void fixedStep();
private:
    std::vector<SimObject> simobjects;
    Camera camera;
    Physics physics;
    Renderer renderer;

    bool forwarddown = false;
    bool backdown = false;
    bool leftdown = false;
    bool rightdown = false;

    const float dtfixed = 1.f/60.f;
    const float move_speed = 2.f; // meters per second
    const float mouse_sensitivity = 0.02f; // radians per pixel
};

#endif
