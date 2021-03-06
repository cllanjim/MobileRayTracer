#include "c_wrapper.h"
#include "Components/Cameras/Orthographic.hpp"
#include "Components/Cameras/Perspective.hpp"
#include "Components/Lights/AreaLight.hpp"
#include "Components/Lights/PointLight.hpp"
#include "Components/ObjectLoaders/OBJLoader.hpp"
#include "Components/Samplers/Constant.hpp"
#include "Components/Samplers/HaltonSeq.hpp"
#include "Components/Samplers/MersenneTwister.hpp"
#include "Components/Samplers/StaticHaltonSeq.hpp"
#include "Components/Samplers/StaticMersenneTwister.hpp"
#include "Components/Samplers/Stratified.hpp"
#include "Components/Shaders/DepthMap.hpp"
#include "Components/Shaders/DiffuseMaterial.hpp"
#include "Components/Shaders/NoShadows.hpp"
#include "Components/Shaders/PathTracer.hpp"
#include "Components/Shaders/Whitted.hpp"
#include "MobileRT/Renderer.hpp"
#include "MobileRT/Scene.hpp"
#include "Scenes.hpp"
#include <chrono>
#include <fstream>

static void
work_thread(
    ::std::uint32_t *const bitmap, const ::std::int32_t width,
    const ::std::int32_t height, const ::std::int32_t threads,
    const ::std::int32_t shader, const ::std::int32_t scene, const ::std::int32_t samplesPixel, const ::std::int32_t samplesLight,
    ::std::int32_t repeats, const ::std::int32_t accelerator, const bool printStdOut,
    const char *const pathObj, const char *const pathMtl) {
    ::std::ostringstream ss{""};
    ::std::streambuf *old_buf_stdout{nullptr};
    ::std::streambuf *old_buf_stderr{nullptr};
    ::std::chrono::duration<double> timeCreating{0};
    ::std::chrono::duration<double> timeRendering{0};
    if (!printStdOut) {
        old_buf_stdout = ::std::cout.rdbuf(ss.rdbuf());
        old_buf_stderr = ::std::cerr.rdbuf(ss.rdbuf());
    }
    {
        LOG("width_ = ", width);
        LOG("height_ = ", height);
        LOG("threads = ", threads);
        LOG("shader = ", shader);
        LOG("scene = ", scene);
        LOG("samplesPixel = ", samplesPixel);
        LOG("samplesLight = ", samplesLight);
        LOG("repeats = ", repeats);
        LOG("accelerator = ", accelerator);
        LOG("printStdOut = ", printStdOut);
        LOG("pathObj = ", pathObj);
        LOG("pathMtl = ", pathMtl);

        ::std::unique_ptr<::MobileRT::Renderer> renderer_ {};
        ::std::ifstream obj {pathObj};
        ::std::ifstream mtl {pathMtl};
        ::std::string line {};
        ::std::stringstream ssObj {""};
        while (::std::getline(obj, line)) {
            ssObj << line << '\n';
        }
        ::std::stringstream ssMtl{""};
        while (::std::getline(mtl, line)) {
            ssMtl << line << '\n';
        }
        ::Components::OBJLoader objLoader {ssObj.str(), ssMtl.str()};
        objLoader.process();
        ::std::int32_t numberOfLights_ {0};

        const float ratio {
            ::std::max(static_cast<float>(width) / height, static_cast<float>(height) / width)};
        const float hfovFactor {width > height ? ratio : 1.0f};
        const float vfovFactor {width < height ? ratio : 1.0f};
        ::MobileRT::Scene scene_ {};
        ::std::unique_ptr<::MobileRT::Sampler> samplerPixel {};
        ::std::unique_ptr<::MobileRT::Shader> shader_ {};
        ::std::unique_ptr<::MobileRT::Camera> camera {};
        ::glm::vec3 maxDist{};

        switch (scene) {
            case 0:
                camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.0f, -3.4f},
                        ::glm::vec3 {0.0f, 0.0f, 1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);
                scene_ = cornellBoxScene(::std::move(scene_));
                maxDist = ::glm::vec3 {1, 1, 1};
                break;

            case 1:
                camera = ::std::make_unique<::Components::Orthographic>(
                        ::glm::vec3 {0.0f, 1.0f, -10.0f},
                        ::glm::vec3 {0.0f, 1.0f, 7.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        10.0f * hfovFactor, 10.0f * vfovFactor);
                /*camera = ::std::make_unique<::Components::Perspective>(
                  ::glm::vec3 {0.0f, 0.5f, 1.0f},
                  ::glm::vec3 {0.0f, 0.0f, 7.0f},
                  ::glm::vec3 {0.0f, 1.0f, 0.0f},
                  60.0f * hfovFactor, 60.0f * vfovFactor);*/
                scene_ = spheresScene(::std::move(scene_));
                maxDist = ::glm::vec3 {8, 8, 8};
                break;

            case 2:
                camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.0f, -3.4f},
                        ::glm::vec3 {0.0f, 0.0f, 1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);
                scene_ = cornellBoxScene2(::std::move(scene_));
                maxDist = ::glm::vec3 {1, 1, 1};
                break;

            case 3:
                camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.5f, 1.0f},
                        ::glm::vec3 {0.0f, 0.0f, 7.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        60.0f * hfovFactor, 60.0f * vfovFactor);
                scene_ = spheresScene2(::std::move(scene_));
                maxDist = ::glm::vec3 {8, 8, 8};
                break;
            default: {
                //objLoader.fillScene (&scene_, []() noexcept -> ::std::unique_ptr<::Components::StaticHaltonSeq> {return ::std::make_unique<::Components::HaltonSeq> ();});
                //objLoader.fillScene (&scene_, []() noexcept -> ::std::unique_ptr<::Components::StaticHaltonSeq> {return ::std::make_unique<::Components::MersenneTwister> ();});
                objLoader.fillScene(&scene_,
                                    []() noexcept -> ::std::unique_ptr<::Components::StaticHaltonSeq> { return ::std::make_unique<::Components::StaticHaltonSeq>(); });
                //objLoader.fillScene (&scene_, []() noexcept -> ::std::unique_ptr<::Components::StaticHaltonSeq> {return ::std::make_unique<::Components::StaticMersenneTwister> ();});
                const ::MobileRT::Material &lightMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                    ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                    ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                    1.0f,
                                                    ::glm::vec3 {0.9f, 0.9f, 0.9f}};
                //conference
                /*scene_.lights_.emplace_back(::std::make_unique<::Components::PointLight>(
                        lightMat, ::glm::vec3 {0.0f, 1000.0f, 0.0f}));*/
                ::std::unique_ptr<MobileRT::Sampler> samplerPoint1 {
                    ::std::make_unique<Components::StaticHaltonSeq>()};
                scene_.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                        lightMat,
                        ::std::move(samplerPoint1),
                        ::glm::vec3 {-100.0f, 640.0f, -100.0f},
                        ::glm::vec3 {100.0f, 640.0f, -100.0f},
                        ::glm::vec3 {100.0f, 640.0f, 100.0f}));
                ::std::unique_ptr<MobileRT::Sampler> samplerPoint2 {
                    ::std::make_unique<Components::StaticHaltonSeq>()};
                scene_.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                        lightMat,
                        ::std::move(samplerPoint2),
                        ::glm::vec3 {-100.0f, 640.0f, -100.0f},
                        ::glm::vec3 {100.0f, 640.0f, 100.0f},
                        ::glm::vec3 {-100.0f, 640.0f, 100.0f}));
                camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {460.0f, 500.0f, -1000.0f},
                        ::glm::vec3 {0.0f, 400.0f, 0.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);
                //cornell spheres
                /*::std::unique_ptr<MobileRT::Sampler> samplerPoint1 {
                    ::std::make_unique<Components::StaticHaltonSeq>()};
                scene_.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                        lightMat,
                        ::std::move(samplerPoint1),
                        ::glm::vec3 {-0.5f, 1.90f, -0.5f},
                        ::glm::vec3 {0.5f, 1.90f, -0.5f},
                        ::glm::vec3 {0.5f, 1.90f, 0.5f}));
                ::std::unique_ptr<MobileRT::Sampler> samplerPoint2 {
                    ::std::make_unique<Components::StaticHaltonSeq>()};
                scene_.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                        lightMat,
                        ::std::move(samplerPoint2),
                        ::glm::vec3 {-0.5f, 1.90f, -0.5f},
                        ::glm::vec3 {0.5f, 1.90f, 0.5f},
                        ::glm::vec3 {-0.5f, 1.90f, 0.5f}));
                camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.7f, 3.0f},
                        ::glm::vec3 {0.0f, 0.7f, -1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);*/
                //teapot
                //camera = ::std::make_unique<::Components::Perspective> (::glm::vec3 {0.0f, 30.0f, -200.0f}, ::glm::vec3 {0.0f, 30.0f, 100.0f}, ::glm::vec3 {0.0f, 1.0f, 0.0f}, 45.0f * hfovFactor, 45.0f * vfovFactor);
                //sponza
                /*camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {460.0f, 100.0f, 0.0f},
                        ::glm::vec3 {0.0f, 100.0f, 0.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);*/
                //powerplant
                /*camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {460.0f, 100.0f, 25000.0f},
                        ::glm::vec3 {0.0f, 100.0f, 25000.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);*/
                //San Miguel
                /*camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.0f, 0.0f},
                        ::glm::vec3 {1.0f, 0.0f, 0.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);*/
                maxDist = ::glm::vec3 {1, 1, 1};
            }
                break;
        }
        if (samplesPixel > 1) {
            //samplerPixel = ::std::make_unique<::Components::HaltonSeq> ();
            //samplerPixel = ::std::make_unique<::Components::Stratified> ();
            samplerPixel = ::std::make_unique<::Components::StaticHaltonSeq>();
        } else {
            samplerPixel = ::std::make_unique<::Components::Constant>(0.5f);
        }
        switch (shader) {
            case 1: {
                shader_ = ::std::make_unique<::Components::Whitted>(::std::move(scene_),
                                                                    samplesLight,
                                                                    ::MobileRT::Shader::Accelerator(
                                                                            accelerator));
                break;
            }

            case 2: {
                //::std::unique_ptr<::MobileRT::Sampler> samplerRussianRoulette {::std::make_unique<::Components::HaltonSeq>()};
                //::std::unique_ptr<::MobileRT::Sampler> samplerRussianRoulette {::std::make_unique<::Components::MersenneTwister> ()};
                ::std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette{
                        ::std::make_unique<::Components::StaticHaltonSeq>()};
                //::std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette {::std::make_unique<::Components::StaticMersenneTwister> ()};

                shader_ = ::std::make_unique<::Components::PathTracer>(
                        ::std::move(scene_), ::std::move(samplerRussianRoulette), samplesLight,
                        ::MobileRT::Shader::Accelerator(accelerator));
                break;
            }

            case 3: {
                shader_ = ::std::make_unique<::Components::DepthMap>(::std::move(scene_), maxDist,
                                                                     ::MobileRT::Shader::Accelerator(
                                                                             accelerator));
                break;
            }

            case 4: {
                shader_ = ::std::make_unique<::Components::DiffuseMaterial>(::std::move(scene_),
                                                                            ::MobileRT::Shader::Accelerator(
                                                                                    accelerator));
                break;
            }

            default: {
                shader_ = ::std::make_unique<::Components::NoShadows>(::std::move(scene_),
                                                                      samplesLight,
                                                                      ::MobileRT::Shader::Accelerator(
                                                                              accelerator));
                break;
            }
        }
        const ::std::int32_t triangles{static_cast<::std::int32_t> (shader_->scene_.triangles_.size())};
        const ::std::int32_t spheres{static_cast<::std::int32_t> (shader_->scene_.spheres_.size())};
        const ::std::int32_t planes{static_cast<::std::int32_t> (shader_->scene_.planes_.size())};
        numberOfLights_ = static_cast<::std::int32_t> (shader_->scene_.lights_.size());
        const ::std::int32_t nPrimitives = triangles + spheres + planes;

        LOG("Started creating Renderer");
        const auto startCreating{::std::chrono::system_clock::now()};
        renderer_ = ::std::make_unique<::MobileRT::Renderer>(
                ::std::move(shader_), ::std::move(camera), ::std::move(samplerPixel),
                static_cast<::std::uint32_t>(width), static_cast<::std::uint32_t>(height),
                static_cast<::std::uint32_t>(samplesPixel));
        const auto endCreating{::std::chrono::system_clock::now()};
        timeCreating = endCreating - startCreating;
        LOG("Renderer created = ", timeCreating.count());

        LOG("TRIANGLES = ", triangles);
        LOG("SPHERES = ", spheres);
        LOG("PLANES = ", planes);
        LOG("PRIMITIVES = ", nPrimitives);
        LOG("LIGHTS = ", numberOfLights_);
        LOG("threads = ", threads);
        LOG("shader = ", shader);
        LOG("scene = ", scene);
        LOG("samplesPixel = ", samplesPixel);
        LOG("samplesLight = ", samplesLight);
        LOG("width_ = ", width);
        LOG("height_ = ", height);

        LOG("Started rendering scene");
        const auto startRendering{::std::chrono::system_clock::now()};
        do {
            renderer_->renderFrame(bitmap, threads, width * sizeof(::std::uint32_t));
            repeats--;
        } while (repeats > 0);
        const auto endRendering{::std::chrono::system_clock::now()};
        timeRendering = endRendering - startRendering;
        LOG("Finished rendering scene");
    }
    if (!printStdOut) {
        ::std::cout.rdbuf(old_buf_stdout);
        ::std::cerr.rdbuf(old_buf_stderr);
    }

    LOG("Creating Time in secs = ", timeCreating.count());
    LOG("Rendering Time in secs = ", timeRendering.count());
}

void RayTrace(::std::uint32_t *const bitmap, const ::std::int32_t width, const ::std::int32_t height, const ::std::int32_t threads,
              const ::std::int32_t shader, const ::std::int32_t scene, const ::std::int32_t samplesPixel, const ::std::int32_t samplesLight,
              const ::std::int32_t repeats, const ::std::int32_t accelerator, const bool printStdOut, const bool async,
              const char *const pathObj, const char *const pathMtl) {
    if (async) {
        ::std::thread thread {work_thread, bitmap, width, height, threads, shader, scene,
                             samplesPixel, samplesLight, repeats, accelerator, printStdOut, pathObj,
                             pathMtl};
        thread.detach();
    } else {
        work_thread(bitmap, width, height, threads, shader, scene, samplesPixel, samplesLight,
                    repeats, accelerator, printStdOut, pathObj, pathMtl);
    }
}
