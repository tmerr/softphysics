#include "boundingbox.hpp"
#include "camera.hpp"
#include "renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#define BOOST_TEST_MODULE MyTests
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(boundingbox_contents) {
    std::vector<glm::vec3> points;
    points.push_back(glm::vec3(-10.f, -20.f, 10.f));
    points.push_back(glm::vec3(30.f, 40.f, 0.f));
    BoundingBox bb = BoundingBox::ofPoints(points);
    BOOST_CHECK_EQUAL(bb.p0.x, -10.f);
    BOOST_CHECK_EQUAL(bb.p0.y, -20.f);
    BOOST_CHECK_EQUAL(bb.p0.z, 0.f);
    BOOST_CHECK_EQUAL(bb.p1.x, 30.f);
    BOOST_CHECK_EQUAL(bb.p1.y, 40.f);
    BOOST_CHECK_EQUAL(bb.p1.z, 10.f);
}

BOOST_AUTO_TEST_CASE(boundingbox_contains) {
    std::vector<glm::vec3> points;
    points.push_back(glm::vec3(-10.f, -20.f, 10.f));
    points.push_back(glm::vec3(30.f, 40.f, 0.f));
    BoundingBox bb = BoundingBox::ofPoints(points);
    BOOST_CHECK(bb.contains(glm::vec3(0.f, 0.f, 0.5f)));
    BOOST_CHECK(!bb.contains(glm::vec3(0.f, 0.f, -1.f)));
}

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

BOOST_AUTO_TEST_CASE(camera_faces_negz) {
    Camera c(glm::vec3(0.f, 0.f, 0.f), 0.f, 0.f, 1.f);
    BOOST_CHECK(approxEqual(c.getFacingVector(), glm::vec3(0.f, 0.f, -1.f)));
}

BOOST_AUTO_TEST_CASE(camera_forward) {
    Camera c(glm::vec3(0.f, 0.f, 0.f), 0.f, 0.f, 1.f);
    c.forward(2.f);
    BOOST_CHECK(approxEqual(c.getLocation(), glm::vec3(0.f, 0.f, -2.f)));
}

BOOST_AUTO_TEST_CASE(camera_strafe) {
    Camera c(glm::vec3(0.f, 0.f, 0.f), 0.f, 0.f, 1.f);
    c.strafe(2.f);
    BOOST_CHECK(approxEqual(c.getLocation(), glm::vec3(2.f, 0.f, 0.f)));
}

BOOST_AUTO_TEST_CASE(camera_rotate) {
    Camera c(glm::vec3(0.f, 0.f, 0.f), 0.f, 0.f, 1.f);
    c.turn(glm::half_pi<float>(), 0.f);
    BOOST_CHECK(approxEqual(c.getFacingVector(), glm::vec3(-1.f, 0.f, 0.f)));
}

BOOST_AUTO_TEST_CASE(camera_worldtoclip1) {
    Camera c(glm::vec3(0.f, 0.f, 0.f), 0.f, 0.f, 1.f);
    glm::mat4 w2c = c.getWorldToClip();
    glm::vec4 inclip = w2c * glm::vec4(0.f, 0.f, -10.f, 1.f);
    inclip /= inclip.w; // perspective divide to go from clip space to ndc
    BOOST_CHECK(approxEqual(inclip.x, 0.f));
    BOOST_CHECK(approxEqual(inclip.y, 0.f));
    BOOST_CHECK(-1.f < inclip.z && inclip.z < 1.f);
    BOOST_CHECK(approxEqual(inclip.w, 1.f));
}

BOOST_AUTO_TEST_CASE(camera_worldtoclip2) {
    Camera c(glm::vec3(0.f, 0.f, 0.f), 0.f, 0.f, 1.f);
    glm::mat4 w2c = c.getWorldToClip();
    glm::vec4 notinclip = w2c * glm::vec4(glm::vec3(0.f, -50.f, -2.f), 1.f);
    notinclip /= notinclip.w; //perspective divide to go from clip space to ndc
    BOOST_CHECK(approxEqual(notinclip.x, 0.f));
    BOOST_CHECK(notinclip.y < -1.f);
    BOOST_CHECK(-1.f < notinclip.z && notinclip.z < 1.f);
    BOOST_CHECK(approxEqual(notinclip.w, 1.f));
}

void error_callback(int error, const char* description)
{
    std::cerr << "error: " << description;
    exit(EXIT_FAILURE);
}

BOOST_AUTO_TEST_CASE(renderer_init) {
    BOOST_CHECK(glfwInit());
    glfwSetErrorCallback(error_callback);
    GLFWwindow* window = glfwCreateWindow(640, 480, "TestWindow", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    BOOST_CHECK_EQUAL(glewInit(), GLEW_OK); // important
    Renderer renderer;
    BOOST_CHECK_NO_THROW(renderer.init());
    glfwDestroyWindow(window);
    glfwTerminate();
}
