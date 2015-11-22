// mainly pure functions for physics.
#include <boundingbox.hpp>
#include <simobject.hpp>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <boost/variant.hpp>


// this function returns whether a ray intersects a triangle.
// the ray is defined by p + t*d.
// the triangle is defined by v0, v1, and v2.
//
// here you can find the code i based this off of as well as a basic explanation
// of how it works:
// http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/ 
//
// if this function returns true, it also says where, at u*v0 + v*v1 + w*v2.
// you can find w because u + v + w = 1.
bool rayIntersectsTriangle(glm::vec3 p, glm::vec3 d, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
                           float* u, float* v) {
    glm::vec3 e1 = v1 - v0;
    glm::vec3 e2 = v2 - v0;

    glm::vec3 h = glm::cross(d, e2);
	float a = glm::dot(e1, h);

	if (a > -0.00001 && a < 0.00001)
		return false;

	float f = 1.f/a;
    glm::vec3 s = p - v0;
    *u = f * glm::dot(s, h);

	if (*u < 0.f || *u > 1.f)
		return false;

    glm::vec3 q = glm::cross(s, e1);
    *v = f * glm::dot(d, q);

	if (*v < 0.f || *u + *v > 1.f)
		return false;

	// at this stage we can compute t to find out where
	// the intersection point is on the line

	float t = f * glm::dot(e2, q);

	if (t > 0.00001) // ray intersection
		return true;

	else // this means that there is a line intersection
		 // but not a ray intersection
		 return false;
}

// gets the bounding box of a simulation object body
class BoundingBoxGrabber
    : public boost::static_visitor<BoundingBox> {
public:
    BoundingBox operator()(const StaticBody &st) const {
        return st.boundingbox;
    }
    BoundingBox operator()(const SoftBody &so) const {
        return so.boundingbox;
    }
};

// gets the points of a simulation object body
class PointsGrabber
    : public boost::static_visitor<std::vector<glm::vec3> > {
public:
    std::vector<glm::vec3> operator()(const StaticBody &st) const {
        return st.points;
    }
    std::vector<glm::vec3> operator()(const SoftBody &so) const {
        return so.points;
    }
};

// gets the faces of a simulation object body
class FacesGrabber
    : public boost::static_visitor<std::vector<std::array<unsigned int, 3> > > {
public:
    std::vector<std::array<unsigned int, 3> > operator()(const StaticBody &st) const {
        return st.faces;
    }
    std::vector<std::array<unsigned int, 3> > operator()(const SoftBody &so) const {
        return so.faces;
    }
};

// determines whether two kinds of simulation objects can collide
class CanCollide
    : public boost::static_visitor<bool> {
public:
    bool operator()(const StaticBody &, const StaticBody &) const {
        return true;
    }
    template <typename T, typename U>
    bool operator()(const T &, const U &) const {
        return false;
    }
};

typedef size_t PointPenetration;

// gets which face in object2 was penetrated.
//
// also, the location of the face penetration is
// u*v1 + v*v2 + w*v3
// where u + v + w = 1
// (so you have the info to calculate w)
struct FacePenetration {
    size_t which;

    float u;
    float v;
};

typedef boost::variant<PointPenetration, FacePenetration> Penetration;

// quick collision check
// returns true if the two objects can collide and their AABBs intersect
bool broadphase(const SimObject & object1,
                const SimObject & object2) {
    if (boost::apply_visitor(CanCollide(), object1.body, object2.body)) {
        auto bb1 = boost::apply_visitor(BoundingBoxGrabber(), object1.body);
        auto bb2 = boost::apply_visitor(BoundingBoxGrabber(), object2.body);
        if (bb1.intersects(bb2)) {
            return true;
        }
    }
    return false;
}

// careful collision check
// returns which vertices of object1 are inside object2.
std::vector<size_t> narrowphase(const SimObject & object1,
                                const SimObject & object2) {
    // cast a ray from the vertex in object1, and count its intersections with
    // the faces of object2. 
    auto points1 = boost::apply_visitor(PointsGrabber(), object1.body);
    auto faces2 = boost::apply_visitor(FacesGrabber(), object2.body);

    // TODO all the things
}

// get the details of each penetration. for each point of object1 inside object2
// we find the shortest vector to the surface of object.
std::vector<Penetration> calcPenetration(const SimObject & object1,
                                         const SimObject & object2,
                                         std::vector<size_t> & pointsInside) {
    // TODO all the things
}

// apply forces to points in object1 and faces/points in object 2 based on the
// penetrations.
std::vector<glm::vec3> mutateForces(const SimObject & object1,
                                    const SimObject & object2,
                                    std::vector<Penetration> penetrations) {
    // TODO all the things
}
