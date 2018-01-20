//
// Created by Tiago on 16-10-2016.
//

#include "Shader.hpp"

using ::MobileRT::BVH;
using ::MobileRT::Camera;
using ::MobileRT::Intersection;
using ::MobileRT::Ray;
using ::MobileRT::RGB;
using ::MobileRT::Primitive;
using ::MobileRT::Shader;

Shader::Shader (Scene &&scene, const unsigned samplesLight, const Accelerator accelerator) noexcept
  :
  scene_ {::std::move (scene)},
  accelerator_ {accelerator},
  samplesLight_ {samplesLight}
{
  this->scene_.triangles_.shrink_to_fit ();
  this->scene_.spheres_.shrink_to_fit ();
  this->scene_.planes_.shrink_to_fit ();
  this->scene_.lights_.shrink_to_fit ();
}

void Shader::initializeAccelerators (Camera *const camera) noexcept {
  Point3D min {RayLengthMax, RayLengthMax, RayLengthMax};
  Point3D max {-RayLengthMax, -RayLengthMax, -RayLengthMax};
  ::std::vector<Primitive<Triangle> *> triangles {convertVector (this->scene_.triangles_)};
  ::std::vector<Primitive<Sphere> *> spheres {convertVector (this->scene_.spheres_)};
  ::std::vector<Primitive<Plane> *> planes {convertVector (this->scene_.planes_)};
  ::std::vector<Primitive<Rectangle> *> rectangles {convertVector (this->scene_.rectangles_)};
  Scene::getBounds<Primitive<Triangle>> (triangles, &min, &max);
  Scene::getBounds<Primitive<Sphere>> (spheres, &min, &max);
  Scene::getBounds<Primitive<Plane>> (planes, &min, &max);
  Scene::getBounds<Primitive<Rectangle>> (rectangles, &min, &max);
  Scene::getBounds (::std::vector<Camera *> {camera}, &min, &max);
  AABB sceneBounds {min - Epsilon, max + Epsilon};
  switch (accelerator_) {
    case Accelerator::REGULAR_GRID: {
      regularGrid_ = RegularGrid {sceneBounds, &scene_, 32};
      break;
    }
    case Accelerator::BOUNDING_VOLUME_HIERARCHY: {
      bvhPlanes_ = ::MobileRT::BVH<MobileRT::Plane> {sceneBounds, scene_.planes_, 1};
      bvhRectangles_ = ::MobileRT::BVH<MobileRT::Rectangle> {sceneBounds, scene_.rectangles_, 1};
      bvhSpheres_ = ::MobileRT::BVH<MobileRT::Sphere> {sceneBounds, scene_.spheres_, 1};
      bvhTriangles_ = ::MobileRT::BVH<MobileRT::Triangle> {sceneBounds, scene_.triangles_, 1};
      LOG("Boxes created = ", MobileRT::counter);
      break;
    }
    case Accelerator::NONE: {
      break;
    }
  }
}

bool Shader::traceTouch (Intersection *const intersection, Ray &&ray) noexcept {
  return this->scene_.trace(intersection, ::std::move(ray));
}

Shader::~Shader () noexcept {
	LOG("SHADER DELETED");
}

bool Shader::shadowTrace (Intersection *const intersection, Ray &&ray) noexcept {
  bool intersected {false};
  switch (accelerator_) {
    case Accelerator::REGULAR_GRID: {
      intersected = this->regularGrid_.shadowTrace (intersection, ::std::move (ray));
      break;
    }
    case Accelerator::BOUNDING_VOLUME_HIERARCHY: {
      intersected = this->bvhPlanes_.shadowTrace (intersection, ray);
      intersected |= this->bvhRectangles_.shadowTrace (intersection, ray);
      intersected |= this->bvhSpheres_.shadowTrace (intersection, ray);
      intersected |= this->bvhTriangles_.shadowTrace (intersection, ray);
      break;
    }
    case Accelerator::NONE: {
      intersected = this->scene_.shadowTrace (intersection, ::std::move (ray));
      break;
    }
  }
  return intersected;
}

bool Shader::rayTrace (RGB *const rgb, Intersection *const intersection, Ray &&ray) noexcept {
  bool intersected {false};
  switch (accelerator_) {
    case Accelerator::REGULAR_GRID: {
      intersected = this->regularGrid_.trace (intersection, ray);
      break;
    }
    case Accelerator::BOUNDING_VOLUME_HIERARCHY: {
      intersected = this->bvhPlanes_.trace (intersection, ray);
      intersected |= this->bvhRectangles_.trace (intersection, ray);
      intersected |= this->bvhSpheres_.trace (intersection, ray);
      intersected |= this->bvhTriangles_.trace (intersection, ray);
      intersected |= this->scene_.traceLights (intersection, ray);
      break;
    }
    case Accelerator::NONE: {
      intersected = this->scene_.trace (intersection, ray);
      break;
    }
  }
  return intersected && shade(rgb, *intersection, ::std::move(ray));
}
