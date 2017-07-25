//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHAPES_SPHERE_HPP
#define MOBILERT_SHAPES_SPHERE_HPP

#include "../Accelerators/AABB.hpp"
#include "../Intersection.hpp"
#include "../Point3D.hpp"
#include "../Ray.hpp"

namespace MobileRT {
    class Sphere final {
    private:
        Point3D center_;
        const float sq_radius_;

		private:
        Point3D getPositionMin() const noexcept;

        Point3D getPositionMax() const noexcept;

    public:
			explicit Sphere(const Point3D &center, float radius) noexcept;

			Sphere(const Sphere &sphere) noexcept = default;

			Sphere(Sphere &&sphere) noexcept = default;

			~Sphere() noexcept = default;

			Sphere &operator=(const Sphere &sphere) noexcept = delete;

			Sphere &operator=(Sphere &&sphere) noexcept = delete;

			bool intersect(Intersection *intersection, const Ray &ray) const noexcept;

			void moveTo(float x, float y) noexcept;

			float getZ() const noexcept;

			AABB getAABB() const noexcept;

			bool intersect(const AABB &box) const noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SHAPES_SPHERE_HPP
