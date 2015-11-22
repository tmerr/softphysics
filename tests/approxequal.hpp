#ifndef APPROXEQUAL_HPP_INCLUDED
#define APPROXEQUAL_HPP_INCLUDED

static const float epsilon = 0.000001f;

bool approxEqual(float f1, float f2) {
    return glm::abs(f2 - f1) < epsilon;
}

bool approxEqual(const glm::vec3 &v1, const glm::vec3 &v2) {
    glm::vec3 delta = glm::abs(v2 - v1);
    return delta.x < epsilon && delta.y < epsilon && delta.z < epsilon;
}

bool approxEqual(const glm::vec4 &v1, const glm::vec4 &v2) {
    glm::vec4 delta = glm::abs(v2 - v1);
    return delta.x < epsilon && delta.y < epsilon && delta.z < epsilon && delta.w < epsilon;
}

#endif
