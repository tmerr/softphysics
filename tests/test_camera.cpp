#include "camera.hpp"
#include "approxequal.hpp"
#include <boost/test/included/unit_test.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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
