//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SPHERE_HPP
#define MOBILERAYTRACER_SPHERE_HPP

#include "Shape.hpp"

namespace MobileRT
{
    class Sphere : public Shape
    {
        private:
            const Point center_;
            const float sq_radius_;

        public:
            Sphere (const Point& center, const float radius);

            bool Intersect(const Ray& ray, const Material* material, Intersection& intersection) override;
    };
}

#endif //MOBILERAYTRACER_SPHERE_HPP