//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAYTRACER_H
#define MOBILERAYTRACER_RAYTRACER_H

#include "Shaders/Shader.h"

namespace MobileRT {
    class Shader;

    class RayTracer {
    private:
        const Scene &scene_;
        const Shader &shader_;
        typedef std::function<void(RGB &, Ray &, Intersection &, Vector3D &)> RayTraceCall;
        RayTraceCall rayTraceCall_;

    public:
        RayTracer(const Scene &scene, const Shader &shader);

        ~RayTracer();

        void rayTrace(RGB &rgb, Ray &ray, Intersection &intersection, Vector3D &vector) const;
    };
}

#endif //MOBILERAYTRACER_RAYTRACER_H