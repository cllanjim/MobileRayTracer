//
// Created by Tiago on 16-10-2016.
//

#include "Intersection.h"

using namespace MobileRT;

Intersection::Intersection() :
        length_(RAY_LENGTH_MAX),
        material_(nullptr) {
}

void Intersection::reset(const Point3D &point, const Vector3D &normal, const float length,
                         const Material &material) {
    this->point_ = point;
    this->normal_ = normal;
    this->length_ = length;
    this->material_ = &material;
}

Vector3D &Intersection::getSymNormal() {
    this->symNormal_.reset(-this->normal_.x_, -this->normal_.y_, -this->normal_.z_);
    return this->symNormal_;
}