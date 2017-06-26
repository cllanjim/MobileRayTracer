//
// Created by puscas on 06-03-2017.
//

#include "AxisAlignedBoundingBox.h"

using namespace MobileRT;

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Point3D &pointMin, const Point3D &pointMax) :
    pointMin_(pointMin), pointMax_(pointMax)
{
}

bool AxisAlignedBoundingBox::intersect(Intersection &, const Ray &ray,
               const Material &) const
{
    float txmin((pointMin_.x_ - ray.origin_.x_) / ray.direction_.x_);
    float txmax((pointMax_.x_ - ray.origin_.x_) / ray.direction_.x_);

    if (txmin > txmax) std::swap(txmin, txmax);

    float tymin ((pointMin_.y_ - ray.origin_.y_) / ray.direction_.y_);
    float tymax ((pointMax_.y_ - ray.origin_.y_) / ray.direction_.y_);

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((txmin > tymax) || (tymin > txmax)) return false;

    if (tymin > txmin) txmin = tymin;

    if (tymax < txmax) txmax = tymax;

    float tzmin ((pointMin_.z_ - ray.origin_.z_) / ray.direction_.z_);
    float tzmax ((pointMax_.z_ - ray.origin_.z_) / ray.direction_.z_);

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((txmin > tzmax) || (tzmin > txmax)) return false;

    if (tzmin > txmin) txmin = tzmin;

    if (tzmax < txmax) txmax = tzmax;

    return true;
}
