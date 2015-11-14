#include "scene.hpp"
#include <glm/vec3.hpp>

Scene::Scene()
    : camera(glm::vec3(0.f), 0.f, 0.f, 1.f) {
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
    float dx_rad = mouse_sensitivity * dy;
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
