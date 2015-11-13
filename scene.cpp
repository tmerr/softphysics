#include "scene.hpp"

void Scene::forwardPressed() { forwarddown = true; }
void Scene::forwardReleased() { forwarddown = false; }
void Scene::backPressed() { backdown = true; }
void Scene::backReleased() { backdown = false; }
void Scene::leftPressed() { leftdown = true; }
void Scene::leftReleased() { leftdown = false; }
void Scene::rightPressed() { rightdown = true;}
void Scene::rightReleased() { rightdown = false; }

void Scene::mouseMoved(int dx, int dy) {
    float dy_rad = - mouse_sensitivity * dx;
    float dx_rad = mouse_sensitivity * dy;
}

void Scene::windowChanged(int width, int height) {
    camera.setAspect((float)width / (float)height);
    renderer.windowChanged(width, height);
}

void Scene::setFixedStep(bool value) {
    fixedstep = value;
}

void Scene::step(float dtparam) {
    float dt = fixedstep ? dtfixed : dtparam;
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
