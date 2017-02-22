//
// Created by Tiago on 15-11-2016.
// Uses: Moller and Trumbore
//

#include "Triangle.h"

using namespace MobileRT;

Triangle::Triangle(const Point3D &pointA, const Point3D &pointB, const Point3D &pointC) :
        pointA_(pointA),
        pointB_(pointB),
        pointC_(pointC),
        AB_(pointB - pointA),
        AC_(pointC - pointA),
        normal_(AB_.crossProduct(AC_)) {
    this->normal_.normalize();
}

bool Triangle::intersect(Intersection &intersection, const Ray &ray,
                         const Material &material) const {
    const Vector3D perpendicularVector(ray.direction_, this->AC_);
    const float normalizedProjection(AB_.dotProduct(perpendicularVector));
    const float abs(normalizedProjection * (1 + (normalizedProjection < 0) * -2));
    if (abs < VECT_PROJ_MIN) return false;

    const float normalizedProjectionInv(1.0f / normalizedProjection);

    Vector3D vertexToCamera(ray.origin_, pointA_);

    const float u(normalizedProjectionInv * vertexToCamera.dotProduct(perpendicularVector));

    if (u < 0.0f || u > 1.0f)
        return false;

    const Vector3D upPerpendicularVector(vertexToCamera, this->AB_);//cross product
    const float v(normalizedProjectionInv * ray.direction_.dotProduct(upPerpendicularVector));

    if (v < 0.0f || (u + v) > 1.0f)
        return false;

    // at this stage we can compute t to find out where
    // the intersection point is on the line
    const float distanceToIntersection(
            normalizedProjectionInv * this->AC_.dotProduct(upPerpendicularVector));

    if (distanceToIntersection < RAY_LENGTH_MIN || distanceToIntersection > ray.maxDistance_)
        return false;

    intersection.reset(
            ray.origin_, ray.direction_, distanceToIntersection,
            this->normal_,
            distanceToIntersection,
            material);
    return true;
}