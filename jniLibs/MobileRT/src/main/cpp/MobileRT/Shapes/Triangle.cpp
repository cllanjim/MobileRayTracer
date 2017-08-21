//
// Created by Tiago on 15-11-2016.
// Uses: Moller and Trumbore
//

#include "Triangle.hpp"

using MobileRT::AABB;
using MobileRT::Triangle;
using MobileRT::Point3D;

Triangle::Triangle (const Point3D pointA, const Point3D pointB, const Point3D pointC,
                    Vector3D normal) noexcept :
  AC_ {pointC - pointA},
  AB_ {pointB - pointA},
  BC_ {pointC - pointB},
  normal_ {normal.isNull () ? AB_.crossProduct (AC_) : normal},
  pointA_ {pointA},
  pointB_ {pointB},
  pointC_ {pointC}
{
}

bool Triangle::intersect (Intersection *const intersection, const Ray ray) const noexcept {
  const Vector3D perpendicularVector {ray.direction_, this->AC_};//cross product
  const float normalizedProjection {this->AB_.dotProduct (perpendicularVector)};
  if (std::fabs (normalizedProjection) < Epsilon) {
		return false;
	}

  const float normalizedProjectionInv {1.0f / normalizedProjection};
  const Vector3D vectorToCamera {ray.origin_, this->pointA_};
  const float u {normalizedProjectionInv * vectorToCamera.dotProduct (perpendicularVector)};
  if (u < 0.0f || u > 1.0f) {
		return false;
	}

  const Vector3D upPerpendicularVector {vectorToCamera, this->AB_};//cross product
  const float v {normalizedProjectionInv * ray.direction_.dotProduct (upPerpendicularVector)};
  if (v < 0.0f || (u + v) > 1.0f) {
    return false;
  }

  // at this stage we can compute t to find out where
  // the intersection point is on the line
  const float distanceToIntersection {
    normalizedProjectionInv * this->AC_.dotProduct (upPerpendicularVector)};

  if (distanceToIntersection < Epsilon || distanceToIntersection > intersection->length_) {
    return false;
	}
  intersection->reset (ray.origin_, ray.direction_, distanceToIntersection, this->normal_);
  return true;
}

void Triangle::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Triangle::getZ() const noexcept {
    return 0.0f;
}

Point3D Triangle::getPositionMin() const noexcept {
  const float x {pointA_.x_ < pointB_.x_ && pointA_.x_ < pointC_.x_ ? pointA_.x_ :
                 pointB_.x_ < pointC_.x_ ? pointB_.x_ :
                 pointC_.x_};
  const float y {pointA_.y_ < pointB_.y_ && pointA_.y_ < pointC_.y_ ? pointA_.y_ :
                 pointB_.y_ < pointC_.y_ ? pointB_.y_ :
                 pointC_.y_};
  const float z {pointA_.z_ < pointB_.z_ && pointA_.z_ < pointC_.z_ ? pointA_.z_ :
                 pointB_.z_ < pointC_.z_ ? pointB_.z_ :
                 pointC_.z_};
  return Point3D (x, y, z);
}

Point3D Triangle::getPositionMax() const noexcept {
  const float x {pointA_.x_ > pointB_.x_ && pointA_.x_ > pointC_.x_ ? pointA_.x_ :
                 pointB_.x_ > pointC_.x_ ? pointB_.x_ : pointC_.x_};
  const float y {pointA_.y_ > pointB_.y_ && pointA_.y_ > pointC_.y_ ? pointA_.y_ :
                 pointB_.y_ > pointC_.y_ ? pointB_.y_ : pointC_.y_};
  const float z {pointA_.z_ > pointB_.z_ && pointA_.z_ > pointC_.z_ ? pointA_.z_ :
                 pointB_.z_ > pointC_.z_ ? pointB_.z_ : pointC_.z_};
  return Point3D (x, y, z);
}

AABB Triangle::getAABB() const noexcept {
  return AABB {getPositionMin (), getPositionMax ()};
}

bool Triangle::intersect (const AABB box) const noexcept {
  std::function<bool (const Point3D, const Vector3D)> intersectRayAABB {
    [&](const Point3D orig, const Vector3D vec) -> bool {
      Vector3D T_1 {};
      Vector3D T_2 {}; // vectors to hold the T-values for every direction
      float t_near {-RayLengthMax}; // maximums defined in float.h
      float t_far {RayLengthMax};
      if (vec.x_ == 0) { // ray parallel to planes in this direction
        if ((orig.x_ < box.pointMin_.x_) || ((orig.x_ + vec.x_) > box.pointMax_.x_)) {
          return false; // parallel AND outside box : no intersection possible
        }
      } else { // ray not parallel to planes in this direction
        T_1.x_ = (box.pointMin_.x_ - orig.x_) / vec.x_;
        T_2.x_ = (box.pointMax_.x_ - orig.x_) / vec.x_;
        if (T_1.x_ > T_2.x_) { // we want T_1 to hold values for intersection with near plane
          std::swap (T_1, T_2);
        }
        if (T_1.x_ > t_near) {
          t_near = T_1.x_;
        }
        if (T_2.x_ < t_far) {
          t_far = T_2.x_;
        }
        if ((t_near > t_far) || (t_far < 0)) {
          return false;
        }
      }
      if (vec.y_ == 0) { // ray parallel to planes in this direction
        if ((orig.y_ < box.pointMin_.y_) || ((orig.y_ + vec.y_) > box.pointMax_.y_)) {
          return false; // parallel AND outside box : no intersection possible
        }
      } else { // ray not parallel to planes in this direction
        T_1.y_ = (box.pointMin_.y_ - orig.y_) / vec.y_;
        T_2.y_ = (box.pointMax_.y_ - orig.y_) / vec.y_;
        if (T_1.y_ > T_2.y_) { // we want T_1 to hold values for intersection with near plane
          std::swap (T_1, T_2);
        }
        if (T_1.y_ > t_near) {
          t_near = T_1.y_;
        }
        if (T_2.y_ < t_far) {
          t_far = T_2.y_;
        }
        if ((t_near > t_far) || (t_far < 0)) {
          return false;
        }
      }
      if (vec.z_ == 0) { // ray parallel to planes in this direction
        if ((orig.z_ < box.pointMin_.z_) || ((orig.z_ + vec.z_) > box.pointMax_.z_)) {
          return false; // parallel AND outside box : no intersection possible
        }
      } else { // ray not parallel to planes in this direction
        T_1.z_ = (box.pointMin_.z_ - orig.z_) / vec.z_;
        T_2.z_ = (box.pointMax_.z_ - orig.z_) / vec.z_;
        if (T_1.z_ > T_2.z_) { // we want T_1 to hold values for intersection with near plane
          std::swap (T_1, T_2);
        }
        if (T_1.z_ > t_near) {
          t_near = T_1.z_;
        }
        if (T_2.z_ < t_far) {
          t_far = T_2.z_;
        }
        if ((t_near > t_far) || (t_far < 0)) {
          return false;
        }
      }
      return true; // if we made it here, there was an intersection - YAY
		}};

		std::function<bool (const Vector3D)> isOverTriangle {
			[&](const Vector3D vec) -> bool {
				const Vector3D perpendicularVector {vec, this->AC_};//cross product
				const float normalizedProjection {this->AB_.dotProduct (perpendicularVector)};
				return std::fabs (normalizedProjection) < Epsilon;
			}
		};



		/*std::function<bool (const Triangle)> intersectRayAABB {
			[&](const Triangle triangle) -> bool {
				double triangleMin, triangleMax;
				double boxMin, boxMax;
		
				// Test the box normals (x-, y- and z-axes)
				Vector3D boxNormals[3] {
					Vector3D (1,0,0),
					Vector3D (0,1,0),
					Vector3D (0,0,1)
				};
				for (int i = 0; i < 3; i++)
				{
						Vector3D n = boxNormals[i];
						Project(triangle.Vertices, boxNormals[i], out triangleMin, out triangleMax);
						if (triangleMax < box.Start.Coords[i] || triangleMin > box.End.Coords[i])
								return false; // No intersection possible.
				}
		
				// Test the triangle normal
				double triangleOffset = triangle.Normal.Dot(triangle.A);
				Project(box.Vertices, triangle.Normal, out boxMin, out boxMax);
				if (boxMax < triangleOffset || boxMin > triangleOffset)
						return false; // No intersection possible.
		
				// Test the nine edge cross-products
				Vector3D[] triangleEdges = Vector3D[3] {
						//triangle.A.Minus(triangle.B),
						//triangle.B.Minus(triangle.C),
						//triangle.C.Minus(triangle.A)
						this->AB_,
						this->AC_,
						this->BC_
				};
				for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
						// The box normals are the same as it's edge tangents
						Vecto3D axis = triangleEdges[i].Cross(boxNormals[j]);
						Project(box.Vertices, axis, out boxMin, out boxMax);
						Project(triangle.Vertices, axis, out triangleMin, out triangleMax);
						if (boxMax <= triangleMin || boxMin >= triangleMax)
								return false; // No intersection possible
				}
		
				// No separating axis found.
				return true;
		};*/

    const Point3D &min (box.pointMin_);
		const Point3D &max (box.pointMax_);
		static_cast<void> (min);
		static_cast<void> (max);
		Intersection intersection {};
		const Vector3D vec {max, min};
		const Ray ray {vec, min, 1};
		const bool intersectedAB {intersectRayAABB(this->pointA_, this->AB_)};
    const bool intersectedAC {intersectRayAABB(this->pointA_, this->AC_)};
    const bool intersectedBC {intersectRayAABB(this->pointB_, this->BC_)};
		const bool intersectedRay {intersect(&intersection, ray)};
		const bool insideTriangle {isOverTriangle (vec)};

    return intersectedAB || intersectedAC || intersectedBC || intersectedRay || insideTriangle;
}
