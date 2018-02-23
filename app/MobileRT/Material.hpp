//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_MATERIAL_HPP
#define MOBILERT_MATERIAL_HPP

#include <glm/glm.hpp>

namespace MobileRT {
    class Material final {
    public:
        glm::vec3 Le_{};   // emission light
        glm::vec3 Kd_{};   // diffuse reflection
        glm::vec3 Ks_{};   // specular reflection
        glm::vec3 Kt_{};   // specular transmission
        float refractiveIndice_{};

    public:
        explicit Material(
            glm::vec3 Kd = glm::vec3 {}, glm::vec3 Ks = glm::vec3 {}, glm::vec3 Kt = glm::vec3 {},
            float refractiveIndice = 1.0f, glm::vec3 Le = glm::vec3 {}) noexcept;

        Material(const Material &material) noexcept = default;

        Material(Material &&material) noexcept = default;

        ~Material() noexcept = default;

        Material &operator=(const Material &material) noexcept = default;

        Material &operator=(Material &&material) noexcept = default;

        static unsigned getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_MATERIAL_HPP
