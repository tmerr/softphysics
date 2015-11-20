#include "scene.hpp"
#include <glm/vec3.hpp>

Scene::Scene(int winwidth, int winheight)
    : camera(glm::vec3(0.f), 0.f, 0.f, winwidth/(float)winheight),
      window(winwidth, winheight, "simulation"),
      renderer(winwidth, winheight)
{
    // triangle body
    StaticBody body;
    body.points.push_back(glm::vec3(-1.f, -0.5f, -3.f));
    body.points.push_back(glm::vec3(1.f, -0.5f, -3.f));
    body.points.push_back(glm::vec3(0.f, 0.5f, -3.f));
    std::array<unsigned int, 3> face1 = {{ 0, 1, 2 }};
    std::array<unsigned int, 3> face2 = {{ 2, 1, 0 }};
    body.faces.push_back(face1);
    body.faces.push_back(face2);

    // triangle material
    SolidMaterial material = { glm::vec3(0.f, 0.f, 1.f) };

    // triangle final object
    SimObject simobject = { body, material };
    simobjects.push_back(simobject);

    window.setMouseMoveCallback([this](float dx, float dy) {
        float dy_rad = - mouse_sensitivity * dx;
        float dx_rad = - mouse_sensitivity * dy;
        camera.turn(dy_rad, dx_rad);
    });

    window.setResizeCallback([this](int width, int height) {
        camera.setAspect((float)width / (float)height);
        renderer.windowChanged(width, height);
    });
}

void Scene::mainLoop() {
    static const float dt = 1.f / 60.f;

    while (!window.shouldClose()) {
        physics.step(simobjects, dt);

        if (window.checkUp()) {
            camera.forward(move_speed * dt);
        }
        if (window.checkDown()) {
            camera.forward(-move_speed * dt);
        }
        if (window.checkLeft()) {
            camera.strafe(-move_speed * dt);
        }
        if (window.checkRight()) {
            camera.strafe(move_speed * dt);
        }

        renderer.render(simobjects, camera.getWorldToClip());

        window.pollEvents();
        window.swapBuffers();
    }
}
