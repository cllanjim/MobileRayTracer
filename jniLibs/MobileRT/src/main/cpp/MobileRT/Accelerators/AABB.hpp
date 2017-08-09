//
// Created by puscas on 06-03-2017.
//

#ifndef MOBILERT_ACCELERATORS_AABB_HPP
#define MOBILERT_ACCELERATORS_AABB_HPP

#include "../Intersection.hpp"
#include "../Material.hpp"
#include "../Ray.hpp"
#include "../Utils.hpp"
#include <vector>

namespace MobileRT {
    class AABB final {
    public:
      Point3D pointMin_ {};
      Point3D pointMax_ {};

    public:
			explicit AABB () noexcept = default;
			explicit AABB (Point3D pointMin,
										 Point3D pointMax) noexcept;

			AABB(const AABB &AABB) noexcept = default;

			AABB(AABB &&AABB) noexcept = default;

			~AABB() noexcept = default;

			AABB &operator=(const AABB &AABB) noexcept = default;

			AABB &operator=(AABB &&AABB) noexcept = default;
    };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_AABB_HPP
