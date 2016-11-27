//
// Created by Tiago on 16-10-2016.
//

#include "Perspective.h"
#include "../Constants.h"

using namespace MobileRT;

Perspective::Perspective(const Point3D &position, const float hFov, const float vFov) :
        position_(position),
        // convert to radians
        hFov_(static_cast<float>(hFov * PI / 180.0f)),
        vFov_(static_cast<float>(vFov * PI / 180.0f)) {
}

void Perspective::getRay(Ray &ray, const float u_alpha, const float v_alpha) const {
    // note that depth will be 0 : primary ray
    // camera a olhar para eixo +Z
    //void recycle (const float x, const float y, const float z, const Point3D& origin);
    ray.recycle(u_alpha, v_alpha, 1.0f, this->position_);
}