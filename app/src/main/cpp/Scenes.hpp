//
// Created by Tiago on 02-Aug-17.
//

#ifndef APP_SCENES_HPP
#define APP_SCENES_HPP

#include "Components/Lights/AreaLight.hpp"
#include "Components/Lights/PointLight.hpp"
#include "Components/Samplers/MersenneTwister.hpp"
#include "MobileRT/Scene.hpp"

MobileRT::Scene cornellBoxScene(MobileRT::Scene&& scene) noexcept;

MobileRT::Scene cornellBoxScene2(MobileRT::Scene&& scene, int samplesLight, int samplesPixel, int width, int height) noexcept;

MobileRT::Scene spheresScene(MobileRT::Scene&& scene) noexcept;

MobileRT::Scene spheresScene2(MobileRT::Scene&& scene) noexcept;

#endif //APP_SCENES_HPP