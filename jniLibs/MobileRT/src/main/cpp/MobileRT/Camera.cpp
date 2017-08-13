//
// Created by Tiago on 23-Jan-17.
//

#include "Camera.hpp"

using MobileRT::Camera;

namespace {
  std::array<float, NumberOfBlocks> VALUES;

  bool FillThings () {
    static std::mt19937 generator (std::random_device{} ());
    for (uint32_t i {0}; i < NumberOfBlocks; ++i) {
      VALUES.at (i) = haltonSequence (i, 2);
    }
    std::shuffle (VALUES.begin (), VALUES.end (), generator);
    return true;
  }
}//namespace

float Camera::getBlock (unsigned sample) noexcept {
  const unsigned current {this->block_.fetch_add (1, std::memory_order_relaxed)};
  if (current >= (NumberOfBlocks * (sample + 1))) {
    this->block_.fetch_sub (1, std::memory_order_relaxed);
    return 1.0f;
  }
  return VALUES.at (current);
}

//Left hand rule
Camera::Camera (Point3D position, Point3D lookAt, Vector3D up) noexcept :
  position_ {position},
  direction_ {Vector3D (lookAt, position, true)},
  right_ {up.crossProduct (direction_)},
  up_ {direction_.crossProduct (right_)} {
  static bool unused {FillThings ()};
  static_cast<void> (unused);
}

Camera::~Camera () noexcept {
	LOG("CAMERA DELETED");
}
