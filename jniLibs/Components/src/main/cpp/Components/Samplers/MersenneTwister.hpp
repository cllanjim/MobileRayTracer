//
// Created by Tiago on 19-04-2017.
//

#ifndef COMPONENTS_SAMPLERS_MERSENNETWISTER_HPP
#define COMPONENTS_SAMPLERS_MERSENNETWISTER_HPP

#include "MobileRT/Sampler.hpp"
#include <random>

namespace Components {
    class MersenneTwister final : public MobileRT::Sampler {
    public:
      explicit MersenneTwister(uint64_t domainSize,
                        unsigned int samples) noexcept;
		
			MersenneTwister(const MersenneTwister &random) noexcept = delete;

			MersenneTwister(MersenneTwister &&random) noexcept = delete;

      ~MersenneTwister() noexcept final = default;

      MersenneTwister &operator=(const MersenneTwister &random) noexcept = delete;

			MersenneTwister &operator=(MersenneTwister &&random) noexcept = delete;

      float getSample(unsigned int sample) noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_SAMPLERS_MERSENNETWISTER_HPP
