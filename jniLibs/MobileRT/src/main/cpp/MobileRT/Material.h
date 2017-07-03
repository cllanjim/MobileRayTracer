//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MATERIAL_H
#define MOBILERAYTRACER_MATERIAL_H

#include "RGB.h"

namespace MobileRT {
    class Material final {
    public:
        const RGB Kd_;   // diffuse reflection
        const RGB Ks_;   // specular reflection
        const RGB Kt_;   // specular transmission
        const RGB Le_;   // auto emission
        const float refractiveIndice_;

    public:
        explicit Material(void) noexcept;

        explicit Material(const RGB &Kd) noexcept;

        explicit Material(const RGB &Kd, const RGB &Ks) noexcept;

        explicit Material(const RGB &Kd, const RGB &Ks, const RGB &Kt) noexcept;

        explicit Material(const RGB &Kd, const RGB &Ks, const RGB &Kt,
                          const RGB &Le) noexcept;

        explicit Material(const RGB &Kd, const float refractiveIndice) noexcept;

        explicit Material(const RGB &Kd, const RGB &Ks,
                          const float refractiveIndice) noexcept;

        explicit Material(const RGB &Kd, const RGB &Ks, const RGB &Kt,
                          const float refractiveIndice) noexcept;

        explicit Material(const RGB &Kd, const RGB &Ks, const RGB &Kt,
                          const RGB &Le, const float refractiveIndice) noexcept;

        static unsigned int getInstances() noexcept;
    };
}

#endif //MOBILERAYTRACER_MATERIAL_H
