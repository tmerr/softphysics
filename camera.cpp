#include "camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 location, float y_rad, float x_rad, float aspect)
    : location(location),
      y_rad(y_rad),
      x_rad(x_rad),
      aspect(aspect) {
}

void Camera::setAspect(float asp) {
    aspect = asp;
}

void Camera::turn(float dy_rad, float dx_rad) {
    y_rad = glm::mod(y_rad + dy_rad, 2.f * glm::pi<float>());
    x_rad = glm::mod(x_rad + dx_rad, 2.f * glm::pi<float>());
}

void Camera::forward(float displacement) {
    location += displacement * getFacingVector();
}

void Camera::strafe(float displacement) {
    location += displacement * glm::cross(getFacingVector(), glm::vec3(0.f, 1.f, 0.f));
}

glm::vec3 Camera::getLocation() {
    return location;
}

glm::vec3 Camera::getFacingVector() {
    static const glm::vec3 negunitz(0.f, 0.f, -1.f);
    return glm::rotateX(glm::rotateY(negunitz, y_rad), x_rad);
}

glm::mat4 Camera::getWorldToClip() {
    auto proj = glm::perspective(fov_rad, aspect, near, far);
    auto viewtrans = glm::lookAt(location,
                                 location + getFacingVector(),
                                 glm::vec3(0.f, 1.f, 0.f));
    return proj * viewtrans;
}
