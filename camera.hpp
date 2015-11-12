#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera {
public:
    Camera(glm::vec3 location, float left_right_angle, float up_down_angle);
    void turn(float left_right_rad, float up_down_rad);
    void forward(float magnitude);
    void strafe(float magnitude);
    glm::vec3 getLocation();
    glm::vec3 getFacingVector();
    glm::mat4x4 getWorldToClip();

private:
    float left_right_angle;
    float up_down_angle;
    glm::vec3 location;
    const float turn_rate = 1.f;
    const float move_rate = 1.f;
    const float fov_rad = 0.61f; // 35 degrees
    const float near = 0.1f;
    const float far = 1000.f;
};

#endif
