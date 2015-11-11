#include "boundingbox.hpp"
#include <vector>
#include <glm/vec3.hpp>

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
