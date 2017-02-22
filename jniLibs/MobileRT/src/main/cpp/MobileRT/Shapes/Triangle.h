//
// Created by Tiago on 15-11-2016.
//

#ifndef MOBILERAYTRACER_TRIANGLE_H
#define MOBILERAYTRACER_TRIANGLE_H

#include "Shape.h"
#include "../Point3D.h"
#include "../Vector3D.h"

namespace MobileRT {
    class Triangle : public Shape {
    public:
        const Point3D pointA_;
        const Point3D pointB_;
        const Point3D pointC_;
        const Vector3D AB_;
        const Vector3D AC_;
        Vector3D normal_;
        char padding[4] __attribute__((unused));

    public:
        explicit Triangle(const Point3D &pointA, const Point3D &pointB, const Point3D &pointC);

        bool intersect(Intersection &intersection, const Ray &ray,
                       const Material &material) const;
    };
}

#endif //MOBILERAYTRACER_TRIANGLE_H