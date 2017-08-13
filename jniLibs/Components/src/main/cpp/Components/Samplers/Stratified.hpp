//
// Created by Tiago on 21-11-2016.
//

#ifndef COMPONENTS_SAMPLERS_STRATIFIED_HPP
#define COMPONENTS_SAMPLERS_STRATIFIED_HPP

#include "MobileRT/Sampler.hpp"

namespace Components {
	class Stratified final : public MobileRT::Sampler {
    public:
    explicit Stratified() noexcept = default;
    explicit Stratified (unsigned width, unsigned height,
                         unsigned samples) noexcept;

		Stratified(const Stratified &stratified) noexcept = delete;

		Stratified(Stratified &&stratified) noexcept = delete;

		~Stratified() noexcept final = default;

		Stratified &operator=(const Stratified &stratified) noexcept = delete;

		Stratified &operator=(Stratified &&stratified) noexcept = delete;
    float getSample (unsigned sample) noexcept final;
	};
}//namespace Components

#endif //COMPONENTS_SAMPLERS_STRATIFIED_HPP
