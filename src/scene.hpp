#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include "camera.hpp"
#include "physics.hpp"
#include "mainwindow.hpp"
#include "renderer.hpp"
#include <vector>

class Scene {
public:
    Scene(int winwidth, int winheight);
    void mainLoop();

private:
    std::vector<SimObject> simobjects;
    Camera camera;
    Physics physics;

    // window must precede renderer
    MainWindow window;
    Renderer renderer;

    const float dtfixed = 1.f/60.f;
    const float move_speed = 2.f; // meters per second
    const float mouse_sensitivity = 0.001f; // radians per pixel
};

#endif
