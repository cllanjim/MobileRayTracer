//
// Created by Tiago on 21-11-2016.
//

#include "MobileRT/Sampler.hpp"

using ::MobileRT::Sampler;

Sampler::Sampler(const ::std::uint32_t width, const ::std::uint32_t height,
                 const ::std::uint32_t samples) noexcept :
        domainSize_{(width / (width / static_cast<::std::uint32_t>(::std::sqrt(NumberOfBlocks)))) *
                    (height / (width / static_cast<::std::uint32_t>(::std::sqrt(NumberOfBlocks))))},
        samples_{samples} {
}

void Sampler::resetSampling() noexcept {
    this->sample_ = 0;
}

void Sampler::stopSampling() noexcept {
    this->samples_ = 0;
}

Sampler::~Sampler() noexcept {
    LOG("SAMPLER DELETED");
}

float Sampler::getSample() noexcept {
    return getSample(0);
}
