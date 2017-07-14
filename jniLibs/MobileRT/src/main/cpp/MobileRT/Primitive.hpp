//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_PRIMITIVE_H
#define MOBILERAYTRACER_PRIMITIVE_H

#include "Intersection.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Shapes/Plane.hpp"
#include "Shapes/Shape.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/Triangle.hpp"

namespace MobileRT {
    class Primitive final {
    private:
        const Material material_;

    public:
        Shape *const shape_;

    public:
        explicit Primitive(Shape *shape, Material material) noexcept;

		Primitive(const Primitive &primitive) noexcept = default;

		Primitive(Primitive &&primitive) noexcept = default;

        ~Primitive() noexcept;

		Primitive &operator=(const Primitive &primitive) const noexcept = delete;

		Primitive &&operator=(Primitive &&primitive) const noexcept = delete;

        bool intersect(Intersection &intersection, const Ray &ray) const noexcept;

        static unsigned int getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_PRIMITIVE_H
