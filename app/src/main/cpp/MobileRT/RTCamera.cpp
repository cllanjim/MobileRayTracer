//
// Created by puscas on 16-10-2016.
//

#include "RTCamera.hpp"
#include "Constants.hpp"

using namespace MobileRT;

RTCamera::RTCamera(const Point& position, const float hFov, const float vFov) :
        // convert to radians
        hFov_(hFov * M_PI / 180.0f),
        vFov_(vFov * M_PI / 180.0f),
        position_(position)
{
}

float RTCamera::FastArcTan(const float x) const
{
    const float abs = (x >= 0)? x : -x;
    return M_PI_4*x - (x *(abs - 1)) * (0.2447 + (0.0663 * abs));
}

Ray RTCamera::getRay(const float u, const float v)
{
    const float u_alpha = hFov_ * (u - 0.5f);
    const float v_alpha = -vFov_ * (v - 0.5f);
    const Vect dir = Vect(FastArcTan(u_alpha), FastArcTan(v_alpha), 1.0f);
    // note that depth will be 0 : primary ray
    return Ray(this->position_, dir);
}