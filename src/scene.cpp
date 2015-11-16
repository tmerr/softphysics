#include "scene.hpp"
#include <glm/vec3.hpp>

Scene::Scene() : camera(glm::vec3(0.f), 0.f, 0.f, 1.f) {
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
}

void Scene::init(int width, int height) {
    camera.setAspect((float)width/(float)height);
    renderer.init(width, height);
}

void Scene::forwardPressed() { forwarddown = true; }
void Scene::forwardReleased() { forwarddown = false; }
void Scene::backPressed() { backdown = true; }
void Scene::backReleased() { backdown = false; }
void Scene::leftPressed() { leftdown = true; }
void Scene::leftReleased() { leftdown = false; }
void Scene::rightPressed() { rightdown = true;}
void Scene::rightReleased() { rightdown = false; }

void Scene::mouseMoved(float dx, float dy) {
    float dy_rad = - mouse_sensitivity * dx;
    float dx_rad = - mouse_sensitivity * dy;
    camera.turn(dy_rad, dx_rad);
}

void Scene::windowChanged(int width, int height) {
    camera.setAspect((float)width / (float)height);
    renderer.windowChanged(width, height);
}

void Scene::step(float dt) {
    physics.step(simobjects, dt);

    if (forwarddown) {
        camera.forward(move_speed * dt);
    }
    if (backdown) {
        camera.forward(-move_speed * dt);
    }
    if (leftdown) {
        camera.strafe(-move_speed * dt);
    }
    if (rightdown) {
        camera.strafe(move_speed * dt);
    }
    renderer.render(simobjects, camera.getWorldToClip());
}

void Scene::fixedStep() {
    step(dtfixed);
}
