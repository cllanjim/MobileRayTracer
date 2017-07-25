//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERT_ACCELERATORS_REGULARGRID_HPP
#define MOBILERT_ACCELERATORS_REGULARGRID_HPP

#include "AABB.hpp"
#include "../Shapes/Triangle.hpp"
#include <vector>

namespace MobileRT {
    class RegularGrid final {
    private:
        //std::vector<AxisAlignedBoundingBox *> *boxes_;

    private:
        //void divideBoundingBox(AxisAlignedBoundingBox &bb);

    public:
        explicit RegularGrid(float minX, float maxX,
                             float minY, float maxY,
                             float minZ, float maxZ,
                             unsigned int numberOfBlocks,
                             const std::vector<Triangle *> &primitives);

		RegularGrid(const RegularGrid &regularGrid) noexcept = delete;

		RegularGrid(RegularGrid &&regularGrid) noexcept = delete;

		~RegularGrid() noexcept = delete;

        RegularGrid &operator=(const RegularGrid &regularGrid) noexcept = delete;

		RegularGrid &operator=(RegularGrid &&regularGrid) noexcept = delete;

        bool intersect(Intersection *intersection, Ray const &ray);
    };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_REGULARGRID_HPP
