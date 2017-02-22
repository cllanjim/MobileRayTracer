//
// Created by Tiago on 16-10-2016.
//

#include "Perspective.h"

using namespace MobileRT;

Perspective::Perspective(const Point3D &position, const float hFov, const float vFov) :
        Camera(position, hFov, vFov) {
}

Perspective::~Perspective(void) {
}

void Perspective::getRay(Ray &ray, const float u_alpha, const float v_alpha) const {
    // note that depth will be 0 : primary ray
    // camera looking at Z+
    ray.reset(u_alpha, v_alpha, 1.0f, this->position_);
}