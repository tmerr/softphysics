#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera {
public:
    Camera(glm::vec3 location, float dy_rad, float dx_rad, float aspect);
    void setAspect(float aspect);
    void turn(float dy_rad, float dx_rad);
    void forward(float magnitude);
    void strafe(float magnitude);
    glm::vec3 getLocation();
    glm::vec3 getFacingVector();
    glm::mat4x4 getWorldToClip();

private:
    float y_rad;
    float x_rad;
    glm::vec3 location;
    float aspect;
    const float turn_rate = 1.f;
    const float move_rate = 1.f;
    const float fov_rad = 0.61f; // 35 degrees
    const float near = 0.1f;
    const float far = 1000.f;
};

#endif
