//
// Created by puscas on 06-03-2017.
//

#include "MobileRT/Accelerators/AABB.hpp"
#include "MobileRT/Utils.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Ray;

AABB::AABB(const ::glm::vec3 &pointMin, const ::glm::vec3 &pointMax) noexcept :
        pointMin_{pointMin}, pointMax_{pointMax} {
}

bool ::MobileRT::intersect(const AABB &box, const Ray &ray) noexcept {
    float t1 {(box.pointMin_[0] - ray.origin_[0])/ray.direction_[0]};
    float t2 {(box.pointMax_[0] - ray.origin_[0])/ray.direction_[0]};

    float tmin {::std::min(t1, t2)};
    float tmax {::std::max(t1, t2)};

    for (::std::int32_t axis {1}; axis < 3; ++axis) {
        const float invDir {1.0f / ray.direction_[axis]};
        t1 = (box.pointMin_[axis] - ray.origin_[axis]) * invDir;
        t2 = (box.pointMax_[axis] - ray.origin_[axis]) * invDir;

        tmin = ::std::max(tmin, ::std::min(t1, t2));
        tmax = ::std::min(tmax, ::std::max(t1, t2));
    }

    const bool intersected {tmax >= ::std::max(tmin, 0.0f)};
    return intersected;
}

::std::int32_t AABB::getLongestAxis() const noexcept {
    const float lengthX {pointMax_[0] - pointMin_[0]};
    const float lengthY {pointMax_[1] - pointMin_[1]};
    const float lengthZ {pointMax_[2] - pointMin_[2]};

    const ::std::int32_t longestAxis {lengthX >= lengthY && lengthX >= lengthZ ? 0 :
                    lengthY >= lengthX && lengthY >= lengthZ ? 1 :
                    2
    };

    return longestAxis;
}

float AABB::getSurfaceArea() const noexcept {
    const float lengthX {pointMax_[0] - pointMin_[0]};
    const float lengthY {pointMax_[1] - pointMin_[1]};
    const float lengthZ {pointMax_[2] - pointMin_[2]};

    const float bottomTopArea {2 * lengthX * lengthZ};
    const float sideAreaXY {2 * lengthX * lengthY};
    const float sideAreaZY {2 * lengthZ * lengthY};

    const float surfaceArea {bottomTopArea + sideAreaXY + sideAreaZY};

    return surfaceArea;
}

namespace MobileRT {
    AABB surroundingBox(const AABB &box1, const AABB &box2) noexcept {
        const ::glm::vec3 &min {
                ::glm::min(box1.pointMin_, box2.pointMin_)};
        const ::glm::vec3 &max {
                ::glm::max(box1.pointMax_, box2.pointMax_)};
        const AABB &res{min, max};

        return res;
    }
}//namespace MobileRT
