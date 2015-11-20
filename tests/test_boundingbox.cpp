#include "boundingbox.hpp"
#include <boost/test/included/unit_test.hpp>
#include <glm/glm.hpp>
#include <vector>

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

BOOST_AUTO_TEST_CASE(boundingbox_intersects1) {
    std::vector<glm::vec3> points = {
        glm::vec3(-1.f, -1.f, -1.f),
        glm::vec3(1.f, 1.f, 1.f)
    };
    BoundingBox bb = BoundingBox::ofPoints(points);
    BOOST_CHECK(bb.intersects(bb));
}

BOOST_AUTO_TEST_CASE(boundingbox_intersects2) {
    std::vector<glm::vec3> points1 = {
        glm::vec3(-2.f, -2.f, -2.f),
        glm::vec3(-1.f, -1.f, -1.f)
    };
    std::vector<glm::vec3> points2 = {
        glm::vec3(1.f, 1.f, 1.f),
        glm::vec3(2.f, 2.f, 2.f)
    };
    BoundingBox bb1 = BoundingBox::ofPoints(points1);
    BoundingBox bb2 = BoundingBox::ofPoints(points2);
    BOOST_CHECK(!bb1.intersects(bb2));
}
