//
// Created by puscas on 03-03-2017.
//

#ifndef MOBILERAYTRACER_AREALIGHT_H
#define MOBILERAYTRACER_AREALIGHT_H

#include "MobileRT/Light.h"
#include "MobileRT/Shapes/Triangle.h"
#include "MobileRT/Sampler.h"
#include "MobileRT/Utils.h"

namespace Components {
    using namespace MobileRT;

    class AreaLight final : public Light, public Triangle {
    private:
        Sampler &samplerPointLight_;

    public:
        explicit AreaLight(const Material &radiance,
                           Sampler &samplerPointLight,
                           const Point3D &pointA,
                           const Point3D &pointB,
                           const Point3D &pointC) noexcept;

        virtual const Point3D getPosition(void) noexcept override final;

        virtual void resetSampling(void) noexcept override final;

        virtual bool
        intersectL(Intersection &intersection, const Ray &ray) const noexcept override final;
    };
}

#endif //MOBILERAYTRACER_AREALIGHT_H
