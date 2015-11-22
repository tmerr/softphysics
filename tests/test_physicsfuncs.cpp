#include "physicsfuncs.hpp"
#include "approxequal.hpp"
#include <boost/test/included/unit_test.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>

// test intersection through the centroid of an equilateral triangle
BOOST_AUTO_TEST_CASE(ray_intersects_triangle_centroid) {
    float b, c;
    float bot = -1.f;
    float height = glm::sqrt(3.f);
    bool intersects = physicsfuncs::rayIntersectsTriangle(
            glm::vec3(0.f, bot + height/3.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(-1.f, bot, -2.f), glm::vec3(0.f, bot + height, -2.f), glm::vec3(1.f, bot, -2.f),
            &b, &c);
    BOOST_CHECK(intersects);
    BOOST_CHECK(approxEqual(b, 1./3.));
    BOOST_CHECK(approxEqual(c, 1./3.));
}

// test intersection through the bottom left corner of a triangle
BOOST_AUTO_TEST_CASE(ray_intersects_triangle_corner) {
    float b, c;
    bool intersects = physicsfuncs::rayIntersectsTriangle(
            glm::vec3(0.0001f, 0.0001f, 0.0f), glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(1.f, 0.f, -1.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(1.f, 1.f, -1.f),
            &b, &c);
    BOOST_CHECK(intersects);
    BOOST_CHECK(.99f <= b && b <= 1.f);
    BOOST_CHECK(0.f <= c && c <= 0.01f);
}

BOOST_AUTO_TEST_CASE(broadphase_bothstatic) {
    // TODO: implement
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(broadphase_should_collide) {
    // TODO: implement
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(broadphase_shouldnt_collide) {
    // TODO: implement
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(narrowphase_should_collide) {
    // TODO: implement
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(narrowphase_shouldnt_collide) {
    // TODO: implement
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(calcPenetration_test) {
    // TODO: implement
    BOOST_CHECK(false);
}


BOOST_AUTO_TEST_CASE(mutateFores_test) {
    // TODO: implement
}
