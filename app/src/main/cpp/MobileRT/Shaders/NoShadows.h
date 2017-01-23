//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_NOSHADOWS_H
#define MOBILERAYTRACER_NOSHADOWS_H

#include "Shader.h"

namespace MobileRT
{
    class NoShadows : public Shader
    {
    private:


    public:
        NoShadows(const Scene &scene);

        void shade(RGB &rgb, Intersection &intersection, const Ray &,
                   Vector3D &vectIntersectCamera, RayTraceCall rayTraceCall) const override;
    };
}

#endif //MOBILERAYTRACER_NOSHADOWS_H