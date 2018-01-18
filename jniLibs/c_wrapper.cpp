#include "c_wrapper.hpp"
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
#include <fstream>
#include <omp.h>

void RayTrace (unsigned* bitmap, int width, int height, int /*stride*/, int threads, int shader, int scene, int samplesPixel, int samplesLight, int repeats, int accelerator) {
	LOG("threads = ", threads);
	LOG("shader = ", shader);
	LOG("scene = ", scene);
	LOG("samplesPixel = ", samplesPixel);
	LOG("samplesLight = ", samplesLight);
	LOG("width_ = ", width);
	LOG("height_ = ", height);
	::MobileRT::Renderer *renderer_ {nullptr};
	::std::ifstream obj {"../app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.obj"};
	::std::string line {};
	::std::stringstream ssObj {};
	while (::std::getline (obj, line)) {
		ssObj << line << ::std::endl;
	}
	::std::ifstream mtl {"../app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.mtl"};
	::std::stringstream ssMtl {};
	while (::std::getline (mtl, line)) {
		ssMtl << line << ::std::endl;
	}
	::Components::OBJLoader objLoader {ssObj.str ().c_str (), ssMtl.str ().c_str ()};
	objLoader.process();
	int64_t numberOfLights_ {0};

	const float ratio {width > height ? static_cast<float>(width) / height : static_cast<float>(height) / width};
    const float hfovFactor {width > height ? ratio : 1.0f};
    const float vfovFactor {width < height ? ratio : 1.0f};
    ::MobileRT::Scene scene_ {};
    ::std::unique_ptr<::MobileRT::Sampler> samplerPixel {};
    ::std::unique_ptr<::MobileRT::Shader> shader_ {};
    ::std::unique_ptr<::MobileRT::Camera> camera {};
    ::MobileRT::Point3D maxDist {::MobileRT::Point3D {0, 0, 0}};

	switch (scene) {
      case 0:
        camera = ::std::make_unique<::Components::Perspective> (
          ::MobileRT::Point3D{0.0f, 0.0f, - 3.4f},
          ::MobileRT::Point3D{0.0f, 0.0f, 1.0f},
          ::MobileRT::Vector3D{0.0f, 1.0f, 0.0f},
          45.0f * hfovFactor, 45.0f * vfovFactor);
        scene_ = cornellBoxScene (::std::move (scene_));
        maxDist = ::MobileRT::Point3D {1, 1, 1};
        break;

      case 1:
        camera = ::std::make_unique<::Components::Orthographic> (
          ::MobileRT::Point3D{0.0f, 1.0f, - 10.0f},
          ::MobileRT::Point3D{0.0f, 1.0f, 7.0f},
          ::MobileRT::Vector3D{0.0f, 1.0f, 0.0f},
          10.0f * hfovFactor, 10.0f * vfovFactor);
        /*camera = ::std::make_unique<::Components::Perspective>(
          ::MobileRT::Point3D(0.0f, 0.5f, 1.0f),
          ::MobileRT::Point3D(0.0f, 0.0f, 7.0f),
          ::MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
          60.0f * hfovFactor, 60.0f * vfovFactor);*/
        scene_ = spheresScene (::std::move (scene_));
        maxDist = ::MobileRT::Point3D {8, 8, 8};
        break;

      case 2:
        camera = ::std::make_unique<::Components::Perspective> (
          ::MobileRT::Point3D {0.0f, 0.0f, -3.4f},
          ::MobileRT::Point3D {0.0f, 0.0f, 1.0f},
          ::MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
          45.0f * hfovFactor, 45.0f * vfovFactor);
        scene_ = cornellBoxScene2 (::std::move (scene_));
        maxDist = ::MobileRT::Point3D {1, 1, 1};
        break;

      case 3:
        camera = ::std::make_unique<::Components::Perspective> (
          ::MobileRT::Point3D {0.0f, 0.5f, 1.0f},
          ::MobileRT::Point3D {0.0f, 0.0f, 7.0f},
          ::MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
          60.0f * hfovFactor, 60.0f * vfovFactor);
        scene_ = spheresScene2 (::std::move (scene_));
        maxDist = ::MobileRT::Point3D {8, 8, 8};
        break;
      default: {
        //objLoader.fillScene (&scene_, [](){return ::std::make_unique<::Components::HaltonSeq> ();});
        //objLoader.fillScene (&scene_, [](){return ::std::make_unique<::Components::MersenneTwister> ();});
        objLoader.fillScene (&scene_, [](){return ::std::make_unique<::Components::StaticHaltonSeq> ();});
        //objLoader.fillScene (&scene_, [](){return ::std::make_unique<::Components::StaticMersenneTwister> ();});
        const ::MobileRT::Material lightMat {::MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                           ::MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                           ::MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                           1.0f,
                                           ::MobileRT::RGB {0.9f, 0.9f, 0.9f}};
        scene_.lights_.emplace_back (new ::Components::PointLight {
          lightMat, ::MobileRT::Point3D {0.0f, 1000.0f, 0.0f}});
        //cornell spheres
        camera = ::std::make_unique<::Components::Perspective> (::MobileRT::Point3D {0.0f, 0.7f, 3.0f},
                                                            ::MobileRT::Point3D {0.0f, 0.7f, -1.0f},
                                                            ::MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
                                                            45.0f * hfovFactor, 45.0f * vfovFactor);
        //teapot
        //camera = ::std::make_unique<::Components::Perspective> (::MobileRT::Point3D {0.0f, 30.0f, -200.0f}, ::MobileRT::Point3D {0.0f, 30.0f, 100.0f}, ::MobileRT::Vector3D {0.0f, 1.0f, 0.0f}, 45.0f * hfovFactor, 45.0f * vfovFactor);
        maxDist = ::MobileRT::Point3D {1, 1, 1};
      }
        break;
    }
    if (samplesPixel > 1) {
      //samplerPixel = ::std::make_unique<::Components::HaltonSeq> ();
      //samplerPixel = ::std::make_unique<::Components::Stratified> ();
      samplerPixel = ::std::make_unique<::Components::StaticHaltonSeq> ();
    } else {
      samplerPixel = ::std::make_unique<::Components::Constant> (0.5f);
    }
    switch (shader) {
      case 1: {
        shader_ = ::std::make_unique<::Components::Whitted> (::std::move (scene_), samplesLight,
                                                         ::MobileRT::Shader::Accelerator (
                                                           accelerator));
        break;
      }

      case 2: {
        //::std::unique_ptr<::MobileRT::Sampler> samplerRay {::std::make_unique<::Components::HaltonSeq>()};
        //::std::unique_ptr<::MobileRT::Sampler> samplerRay {::std::make_unique<::Components::MersenneTwister>()};
        ::std::unique_ptr<::MobileRT::Sampler> samplerRay {::std::make_unique<::Components::StaticHaltonSeq> ()};
        //::std::unique_ptr<::MobileRT::Sampler> samplerRay {::std::make_unique<::Components::StaticMersenneTwister>()};

        //::std::unique_ptr<::MobileRT::Sampler> samplerLight {::std::make_unique<::Components::HaltonSeq>()};
        //::std::unique_ptr<::MobileRT::Sampler> samplerLight {::std::make_unique<::Components::MersenneTwister>()};
        ::std::unique_ptr<::MobileRT::Sampler> samplerLight {::std::make_unique<::Components::StaticHaltonSeq> ()};
        //::std::unique_ptr<::MobileRT::Sampler> samplerLight {::std::make_unique<::Components::StaticMersenneTwister>()};

        //std::unique_ptr<::MobileRT::Sampler> samplerRussianRoulette {std::make_unique<::Components::HaltonSeq>()};
        //std::unique_ptr<::MobileRT::Sampler> samplerRussianRoulette {::std::make_unique<::Components::MersenneTwister> ()};
        ::std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette {::std::make_unique<::Components::StaticHaltonSeq> ()};
        //::std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette {::std::make_unique<::Components::StaticMersenneTwister> ()};

        shader_ = ::std::make_unique<::Components::PathTracer> (
          ::std::move (scene_), ::std::move (samplerRay), ::std::move (samplerLight),
          ::std::move (samplerRussianRoulette), samplesLight,
          ::MobileRT::Shader::Accelerator (accelerator));
        break;
      }

      case 3: {
        shader_ = ::std::make_unique<::Components::DepthMap> (::std::move (scene_), maxDist,
                                                          ::MobileRT::Shader::Accelerator (
                                                            accelerator));
        break;
      }

      case 4: {
        shader_ = ::std::make_unique<::Components::DiffuseMaterial> (::std::move (scene_),
                                                                 ::MobileRT::Shader::Accelerator (
                                                                   accelerator));
        break;
      }

      default: {
        shader_ = ::std::make_unique<::Components::NoShadows> (::std::move (scene_), samplesLight,
                                                           ::MobileRT::Shader::Accelerator (
                                                             accelerator));
        break;
      }
    }

  LOG("Started creating Renderer");
  renderer_ = new ::MobileRT::Renderer {
    ::std::move (shader_), ::std::move (camera), ::std::move (samplerPixel),
    static_cast<unsigned>(width), static_cast<unsigned>(height),
    static_cast<unsigned>(samplesPixel)};
  LOG("Renderer created");

	const int64_t triangles {static_cast<int64_t> (renderer_->shader_->scene_.triangles_.size ())};
  const int64_t spheres {static_cast<int64_t> (renderer_->shader_->scene_.spheres_.size ())};
  const int64_t planes {static_cast<int64_t> (renderer_->shader_->scene_.planes_.size ())};
  numberOfLights_ = static_cast<int64_t> (renderer_->shader_->scene_.lights_.size ());
  const int64_t nPrimitives = triangles + spheres + planes;

  LOG("start rendering scene");
	const double start {omp_get_wtime ()};
	do {
		renderer_->renderFrame (bitmap, threads);
		//renderer_->camera_->position_.x_ += 2.0f;
	} while (repeats-- > 1);
  const double time {omp_get_wtime () - start};
  LOG("finished rendering scene");

	delete renderer_;

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
  LOG("Time in secs = ", time);
}