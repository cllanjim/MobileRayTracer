//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RTCAMERA_HPP
#define MOBILERAYTRACER_RTCAMERA_HPP

#include "Point.hpp"
#include "Ray.hpp"

namespace MobileRT
{
    class RTCamera
    {
        private:
            const float hFov_;
            const float vFov_;
            Point position_;

        public:
            RTCamera(const Point& position, const float hFov, const float vFov);
            float FastArcTan(const float x) const;
            void getRay(const float u, const float v, Ray& ray) const;
    };
}

#endif //MOBILERAYTRACER_RTCAMERA_HPP