//
// Created by Tiago on 16-10-2016.
//

#include "Material.hpp"
#include <utility>

using ::MobileRT::Material;
static unsigned counter{0};

Material::Material(glm::vec3 Kd, glm::vec3 Ks, glm::vec3 Kt, const float refractiveIndice, glm::vec3 Le) noexcept :
        Le_{::std::move(Le)},
        Kd_{::std::move(Kd)},
        Ks_{::std::move(Ks)},
        Kt_{::std::move(Kt)},
        refractiveIndice_{refractiveIndice} {
    counter++;
}

unsigned Material::getInstances() noexcept {
    const unsigned res{counter};
    counter = 0;
    return res;
}
