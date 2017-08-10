//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.hpp"

using MobileRT::Sampler;

Sampler::Sampler(const unsigned int width, const unsigned int height, const unsigned int samples,
                 const unsigned int blockSizeX, const unsigned int blockSizeY) noexcept :
  domainSize_ {(width / blockSizeX) * (height / blockSizeY)},
  samples_ {samples}
{
}

void Sampler::resetSampling() noexcept {
  this->sample_ = 0;
}

void Sampler::stopSampling() noexcept {
    this->samples_ = 0;
}

Sampler::~Sampler () noexcept {
	LOG("SAMPLER DELETED");
}

float Sampler::getSample () noexcept {
  return getSample (0);
}
