//
// Created by Tiago on 16-10-2016.
//

#include "MobileRT/Material.hpp"
#include <utility>

using ::MobileRT::Material;

Material::Material(
        const ::glm::vec3 &Kd, const ::glm::vec3 &Ks,
        const ::glm::vec3 &Kt, const float refractiveIndice,
        const ::glm::vec3 &Le) noexcept :
        Le_{Le},
        Kd_{Kd},
        Ks_{Ks},
        Kt_{Kt},
        refractiveIndice_{refractiveIndice} {
}
