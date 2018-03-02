//
// Created by puscas on 03-03-2017.
//

#ifndef COMPONENTS_LIGHTS_AREALIGHT_HPP
#define COMPONENTS_LIGHTS_AREALIGHT_HPP

#include "MobileRT/Light.hpp"
#include "MobileRT/Sampler.hpp"
#include "MobileRT/Shapes/Triangle.hpp"
#include "MobileRT/Utils.hpp"
#include <memory>

namespace Components {

    class AreaLight final : public ::MobileRT::Light {
    private:
        ::MobileRT::Triangle triangle_;
        ::std::unique_ptr<::MobileRT::Sampler> samplerPointLight_{};

    public:
        explicit AreaLight(
            ::MobileRT::Material radiance,
            ::std::unique_ptr<::MobileRT::Sampler> samplerPointLight,
            ::glm::vec3 pointA,
            ::glm::vec3 pointB,
            ::glm::vec3 pointC) noexcept;

        AreaLight(const AreaLight &areaLight) noexcept = delete;

        AreaLight(AreaLight &&areaLight) noexcept = delete;

        ~AreaLight() noexcept final = default;

        AreaLight &operator=(const AreaLight &areaLight) noexcept = delete;

        AreaLight &operator=(AreaLight &&areaLight) noexcept = delete;

        ::glm::vec3 getPosition() noexcept final;

        void resetSampling() noexcept final;

        ::MobileRT::Intersection intersect(
            ::MobileRT::Intersection intersection,
            ::MobileRT::Ray ray) const noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_LIGHTS_AREALIGHT_HPP