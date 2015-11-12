#include "camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 location, float left_right_angle, float up_down_angle)
    : location(location),
      left_right_angle(left_right_angle),
      up_down_angle(up_down_angle) {
}

void Camera::turn(float x, float y) {
    left_right_angle = glm::mod(left_right_angle + x, 2.f * glm::pi<float>());
    up_down_angle = glm::mod(up_down_angle + y, 2.f * glm::pi<float>());
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
    return glm::rotateX(glm::rotateY(negunitz, left_right_angle), up_down_angle);
}

glm::mat4 Camera::getWorldToClip() {
    auto proj = glm::perspective(fov_deg, 1.f, near, far);
    auto viewtrans = glm::lookAt(location,
                                 location + getFacingVector(),
                                 glm::vec3(0.f, 1.f, 0.f));
    return proj * viewtrans;
}
