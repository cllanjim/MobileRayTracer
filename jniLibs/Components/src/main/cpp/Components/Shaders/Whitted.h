//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_WHITTED_H
#define MOBILERAYTRACER_WHITTED_H

#include "MobileRT/Shader.h"

namespace Components {
    using namespace MobileRT;

    class Whitted final : public Shader {
    private:
        virtual void shade(RGB &rgb,
                           const Intersection &intersection,
                           const Ray &ray) const noexcept override final;

    public:
        explicit Whitted(Scene &scene, const unsigned int samplesLight) noexcept;

        virtual void resetSampling(void) noexcept override final;
    };
}

#endif //MOBILERAYTRACER_WHITTED_H
