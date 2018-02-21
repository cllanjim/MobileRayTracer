//
// Created by Tiago on 17/08/17.
//

#ifndef MOBILERT_PRIMITIVE_HPP
#define MOBILERT_PRIMITIVE_HPP

#include "Accelerators/AABB.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Shapes/Plane.hpp"
#include "Shapes/Rectangle.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/Triangle.hpp"

namespace MobileRT {
    using ::MobileRT::Material;

    template<typename T>
    class Primitive final {
    public:
        T shape_{};
        Material material_{};
        int32_t lastRayID_{};

    public:
        Primitive(T shape, Material material) noexcept;

        Primitive() noexcept = delete;

        Primitive(const Primitive &primitive) noexcept = default;

        Primitive(Primitive &&primitive) noexcept = default;

        ~Primitive() noexcept = default;

        Primitive &operator=(const Primitive &primitive) noexcept = default;

        Primitive &operator=(Primitive &&primitive) noexcept = default;

        AABB getAABB() const noexcept;

        Intersection intersect(Intersection intersection, Ray ray) noexcept;
    };


    using MobileRT::Primitive;
    using MobileRT::AABB;


    template<typename T>
    Primitive<T>::Primitive(T shape, Material material) noexcept :
                    shape_{shape},
                    material_{::std::move(material)} {
    }

    template<typename T>
    AABB Primitive<T>::getAABB() const noexcept {
                return this->shape_.getAABB();
    }


    template<typename T>
    Intersection Primitive<T>::intersect(Intersection intersection, const Ray ray) noexcept {
        if (this->lastRayID_ != ray.id_) {
            const float dist {intersection.length_};
            intersection = this->shape_.intersect(intersection, ray);
            if (intersection.length_ < dist) {
                intersection.material_ = &this->material_;
                return intersection;
            }
        }
        lastRayID_ = ray.id_;
        return intersection;
    }



    template<typename T>
    bool intersect(Primitive<T> primitive, const AABB box) noexcept {
        return primitive.shape_.intersect(box);
    }


}//namespace MobileRT

#endif //MOBILERT_PRIMITIVE_HPP