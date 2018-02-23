//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERT_LIGHT_HPP
#define MOBILERT_LIGHT_HPP

#include "Intersection.hpp"
#include "Ray.hpp"
#include <glm/glm.hpp>

namespace MobileRT {
    class Light {
    public:
        const Material radiance_{};

    public:
        explicit Light(Material radiance) noexcept;

        Light(const Light &light) noexcept = delete;

        Light(Light &&light) noexcept = delete;

        virtual ~Light() noexcept;

        Light &operator=(const Light &light) noexcept = delete;

        Light &operator=(Light &&light) noexcept = delete;

        virtual glm::vec3 getPosition() noexcept = 0;

        virtual void resetSampling() noexcept = 0;

        virtual Intersection intersect(Intersection intersection, Ray ray) const noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERT_LIGHT_HPP
