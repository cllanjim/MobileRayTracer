//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_VECTOR3D_H
#define MOBILERAYTRACER_VECTOR3D_H

#include "Point3D.h"

namespace MobileRT
{
    class Vector3D
    {
        private:
            

        public:
            float x_;
            float y_;
            float z_;

            Vector3D ();
            Vector3D (const float x, const float y, const float z);
            Vector3D (const Vector3D& vector);

            float normalize ();
            Vector3D returnNormalized () const;

            // symetric vector
            Vector3D symmetric () const;

            // dot product
            float dotProduct (const Vector3D& vector) const;

            float dot() const;
            void mult (const float value);
            void sub (const Vector3D& vector);
            float length ();

            Vector3D operator*(const float value) const;

            void setVect (const float x, const float y, const float z);
            void setVect (const Point3D& dest, const Point3D& orig);
    };
}

#endif //MOBILERAYTRACER_VECTOR3D_H