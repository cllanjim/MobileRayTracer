#include "Components/src/main/cpp/Components/Cameras/Orthographic.hpp"
#include "Components/src/main/cpp/Components/Cameras/Perspective.hpp"
#include "Components/src/main/cpp/Components/Lights/AreaLight.hpp"
#include "Components/src/main/cpp/Components/Lights/PointLight.hpp"
#include "Components/src/main/cpp/Components/ObjectLoaders/OBJLoader.hpp"
#include "Components/src/main/cpp/Components/Samplers/Constant.hpp"
#include "Components/src/main/cpp/Components/Samplers/HaltonSeq.hpp"
#include "Components/src/main/cpp/Components/Samplers/MersenneTwister.hpp"
#include "Components/src/main/cpp/Components/Samplers/Stratified.hpp"
#include "Components/src/main/cpp/Components/Shaders/DepthMap.hpp"
#include "Components/src/main/cpp/Components/Shaders/DiffuseMaterial.hpp"
#include "Components/src/main/cpp/Components/Shaders/NoShadows.hpp"
#include "Components/src/main/cpp/Components/Shaders/PathTracer.hpp"
#include "Components/src/main/cpp/Components/Shaders/Whitted.hpp"
#include "MobileRT/src/main/cpp/MobileRT/Renderer.hpp"
#include "MobileRT/src/main/cpp/MobileRT/Scene.hpp"
#include "MobileRT/src/main/cpp/MobileRT/Shapes/Plane.hpp"
#include "MobileRT/src/main/cpp/MobileRT/Shapes/Sphere.hpp"
#include "MobileRT/src/main/cpp/MobileRT/Shapes/Triangle.hpp"
#include <cstdint>
#include <fstream>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <iostream>
#include <omp.h>

static MobileRT::Camera *camera_(nullptr);
static MobileRT::Shader *shader_(nullptr);
static MobileRT::Sampler *samplerCamera_(nullptr);
static MobileRT::Sampler *samplerPixel_(nullptr);
static MobileRT::Sampler *samplerRay_(nullptr);
static MobileRT::Sampler *samplerLight_(nullptr);
static MobileRT::Sampler *samplerRussianRoulette_(nullptr);
static MobileRT::Sampler *samplerPointLight_(nullptr);
static MobileRT::Renderer *renderer_(nullptr);
static unsigned int blockSize_(4u);
static unsigned int blockSizeX_(0u);
static unsigned int blockSizeY_(0u);
static unsigned int samplesPixel_(0u);
static unsigned int samplesLight_(0u);
static unsigned int width_(0u);
static unsigned int height_(0u);
static unsigned int *bitmap;
static unsigned char *buffer;//RGBA

static MobileRT::Scene cornellBoxScene(MobileRT::Scene&& scene) noexcept {
    // point light - white
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.lights_.emplace_back(new Components::PointLight(lightMat,
                                                          MobileRT::Point3D(0.0f, 0.99f, 0.0f)));

		// triangle - yellow
    const MobileRT::Material yellowMat(MobileRT::RGB(0.9f, 0.9f, 0.0f));
    scene.triangles_.emplace_back(
            MobileRT::Point3D(0.5f, -0.5f, 0.99f), MobileRT::Point3D(-0.5f, -0.5f, 0.99f),
            MobileRT::Point3D(0.5f, 0.5f, 1.001f));
	scene.materials_.emplace_back(yellowMat);

		// sphere - mirror
    const MobileRT::Material MirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.spheres_.emplace_back(
            MobileRT::Point3D(0.45f, -0.65f, 0.4f), 0.35f);
		scene.materials_.emplace_back(MirrorMat);

    // sphere - green
    const MobileRT::Material GreenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.2f, 0.0f));
    scene.spheres_.emplace_back(
            MobileRT::Point3D(-0.45f, -0.1f, 0.0f), 0.35f);
		scene.materials_.emplace_back(GreenMat);

    // back wall - white
    const MobileRT::Material lightGrayMat(MobileRT::RGB(0.7f, 0.7f, 0.7f));
    scene.planes_.emplace_back(
            MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f));
		scene.materials_.emplace_back(lightGrayMat);

    // front wall - light blue
    const MobileRT::Material lightBlueMat(MobileRT::RGB(0.0f, 0.9f, 0.9f));
    scene.planes_.emplace_back( 
            MobileRT::Point3D(0.0f, 0.0f, -3.5f), MobileRT::Vector3D(0.0f, 0.0f, 1.0f));
						scene.materials_.emplace_back(lightBlueMat);

    // floor - white
    scene.planes_.emplace_back(
            MobileRT::Point3D(0.0f, -1.0f, 0.0f), MobileRT::Vector3D(0.0f, 1.0f, 0.0f));
		scene.materials_.emplace_back(lightGrayMat);

    // ceiling - white
    scene.planes_.emplace_back( 
            MobileRT::Point3D(0.0f, 1.0f, 0.0f), MobileRT::Vector3D(0.0f, -1.0f, 0.0f));
		scene.materials_.emplace_back(lightGrayMat);

    // left wall - red
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    scene.planes_.emplace_back(
            MobileRT::Point3D(-1.0f, 0.0f, 0.0f), MobileRT::Vector3D(1.0f, 0.0f, 0.0f));
		scene.materials_.emplace_back(redMat);

    // right wall - blue
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    scene.planes_.emplace_back(
            MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f));
		scene.materials_.emplace_back(blueMat);

    return std::move(scene);
}

static MobileRT::Scene cornellBoxScene2(MobileRT::Scene&& scene) noexcept {
    LOG("samplesLight = ", samplesLight_, " max = ", static_cast<uint64_t> (-1));
    const uint64_t domainPointLight(/*roundUpPower2*/
            ((width_ * height_ * 2ull) * 2ull * samplesLight_ * samplesPixel_ * RAY_DEPTH_MAX));
    LOG("domainPointLight = ", domainPointLight);
    LOG("width_ = ", width_);
    LOG("height_ = ", height_);
    LOG("samplesLight_ = ", samplesLight_);
    LOG("samplesPixel_ = ", samplesPixel_);
    LOG("RAY_DEPTH_MAX = ", RAY_DEPTH_MAX);
    //samplerPointLight_ = new Components::HaltonSeq(domainPointLight, 1);
    samplerPointLight_ = new Components::MersenneTwister(domainPointLight, 1);

    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));

    scene.lights_.emplace_back(new Components::AreaLight(lightMat,
                                                         samplerPointLight_,
                                                         MobileRT::Point3D(-0.25f, 0.99f, -0.25f),
                                                         MobileRT::Point3D(0.25f, 0.99f, -0.25f),
                                                         MobileRT::Point3D(0.25f, 0.99f, 0.25f)));

    scene.lights_.emplace_back(new Components::AreaLight(lightMat,
                                                         samplerPointLight_,
                                                         MobileRT::Point3D(0.25f, 0.99f, 0.25f),
                                                         MobileRT::Point3D(-0.25f, 0.99f, 0.25f),
                                                         MobileRT::Point3D(-0.25f, 0.99f, -0.25f)));

		scene.lights_.emplace_back(
			new Components::PointLight(lightMat,
         MobileRT::Point3D(0.70f, 0.99f, 0.0f)));

		    // triangle - yellow
    const MobileRT::Material yellowMat(MobileRT::RGB(0.9f, 0.9f, 0.0f));
    scene.triangles_.emplace_back(
            MobileRT::Point3D(0.5f, -0.5f, 0.99f), MobileRT::Point3D(-0.5f, -0.5f, 0.99f),
            MobileRT::Point3D(0.5f, 0.5f, 0.99f));
		scene.materials_.emplace_back(yellowMat);

    // triangle - green
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    scene.triangles_.emplace_back(
            MobileRT::Point3D(-0.5f, 0.5f, 0.99f),
            MobileRT::Point3D(0.5f, 0.5f, 0.99f),
            MobileRT::Point3D(-0.5f, -0.5f, 0.99f));
		scene.materials_.emplace_back(greenMat);

		    // sphere - mirror
    const MobileRT::Material MirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.spheres_.emplace_back(
            MobileRT::Point3D(0.45f, -0.65f, 0.4f), 0.35f);
		scene.materials_.emplace_back(MirrorMat);

    // sphere - transmission
    const MobileRT::Material TransmissionMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                             MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                             MobileRT::RGB(0.9f, 0.9f, 0.9f), 1.9f);
    scene.spheres_.emplace_back(
            MobileRT::Point3D(-0.4f, -0.3f, 0.0f), 0.35f);
		scene.materials_.emplace_back(TransmissionMat);

    // back wall - white
    const MobileRT::Material lightGrayMat(MobileRT::RGB(0.7f, 0.7f, 0.7f));
    scene.planes_.emplace_back( 
            MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f));
		scene.materials_.emplace_back(lightGrayMat);

    // front wall - light blue
    const MobileRT::Material lightBlueMat(MobileRT::RGB(0.0f, 0.9f, 0.9f));
    scene.planes_.emplace_back(
            MobileRT::Point3D(0.0f, 0.0f, -4.0f), MobileRT::Vector3D(0.0f, 0.0f, 1.0f));
		scene.materials_.emplace_back(lightBlueMat);

    // floor - white
    scene.planes_.emplace_back(
            MobileRT::Point3D(0.0f, -1.0f, 0.0f), MobileRT::Vector3D(0.0f, 1.0f, 0.0f));
		scene.materials_.emplace_back(lightGrayMat);
    // ceiling - white
    scene.planes_.emplace_back(
            MobileRT::Point3D(0.0f, 1.0f, 0.0f), MobileRT::Vector3D(0.0f, -1.0f, 0.0f));
		scene.materials_.emplace_back(lightGrayMat);

    // left wall - red
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    scene.planes_.emplace_back(
            MobileRT::Point3D(-1.0f, 0.0f, 0.0f), MobileRT::Vector3D(1.0f, 0.0f, 0.0f));
		scene.materials_.emplace_back(redMat);

    // right wall - blue
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    scene.planes_.emplace_back(
            MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f));
		scene.materials_.emplace_back(blueMat);

    return std::move(scene);
}

static MobileRT::Scene spheresScene(MobileRT::Scene&& scene) noexcept {
    // create one light source
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.lights_.emplace_back(new Components::PointLight(lightMat,
                                                          MobileRT::Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const MobileRT::Material sandMat(MobileRT::RGB(0.914f, 0.723f, 0.531f));
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    const MobileRT::Material mirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene.spheres_.emplace_back(MobileRT::Point3D(-1.0f, 1.0f, 6.0f), 1.0f);
		scene.materials_.emplace_back(redMat);
    scene.spheres_.emplace_back(MobileRT::Point3D(1.5f, 2.0f, 7.0f), 1.0f);
		scene.materials_.emplace_back(mirrorMat);
    scene.spheres_.emplace_back(MobileRT::Point3D(0.0f, 0.5f, 4.5f), 0.5f);
		scene.materials_.emplace_back(greenMat);

		scene.planes_.emplace_back(MobileRT::Point3D(0.0f, 0.0f, 0.0f),
       MobileRT::Vector3D(0.0f, 1.0f, 0.0f));
		scene.materials_.emplace_back(sandMat);
    return std::move(scene);
}

static MobileRT::Scene spheresScene2(MobileRT::Scene&& scene) noexcept {
    // create one light source
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.lights_.emplace_back(new Components::PointLight(lightMat,
                                                          MobileRT::Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const MobileRT::Material sandMat(MobileRT::RGB(0.914f, 0.723f, 0.531f));
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    const MobileRT::Material yellowMat(MobileRT::RGB(0.9f, 0.9f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.4f));
    const MobileRT::Material mirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene.spheres_.emplace_back(MobileRT::Point3D(-1.0f, 1.0f, 6.0f), 1.0f);
		scene.materials_.emplace_back(redMat);
    scene.spheres_.emplace_back(MobileRT::Point3D(-1.0f, 2.0f, 5.0f), 0.3f);
		scene.materials_.emplace_back(blueMat);
    scene.spheres_.emplace_back(MobileRT::Point3D(1.5f, 2.0f, 7.0f), 1.0f);
		scene.materials_.emplace_back(mirrorMat);
    scene.spheres_.emplace_back(MobileRT::Point3D(1.5f, 0.5f, 5.0f), 0.2f);
		scene.materials_.emplace_back(yellowMat);
    scene.spheres_.emplace_back(MobileRT::Point3D(0.0f, 0.5f, 4.5f), 0.5f);
		scene.materials_.emplace_back(greenMat);

		scene.planes_.emplace_back(MobileRT::Point3D(0.0f, 0.0f, 0.0f),
       MobileRT::Vector3D(0.0f, 1.0f, 0.0f));
		scene.materials_.emplace_back(sandMat);
    return std::move(scene);
}

int main(int argc, char **argv) noexcept {
    int repeats(atoi(argv[4]));
    const int scene(atoi(argv[3]));
    const int shader(atoi(argv[2]));
    const int threads(atoi(argv[1]));
    const int sampler(0);
    const int samplesPixel(atoi(argv[5]));
		const int samplesLight(atoi(argv[6]));
		width_ = static_cast<unsigned int> (atoi(argv[7]));
		height_ = static_cast<unsigned int> (atoi(argv[8]));
		bitmap = new unsigned int [width_ * height_];
		buffer = new unsigned char [width_ * height_ * 4u];
		const float ratio(static_cast<float>(height_) / static_cast<float>(width_));
		camera_ = nullptr;
		shader_ = nullptr;
		samplerCamera_ = nullptr;
		samplerPixel_ = nullptr;
		samplerRay_ = nullptr;
		samplerLight_ = nullptr;
    samplerRussianRoulette_ = nullptr;
		samplerPointLight_ = nullptr;
		renderer_ = nullptr;
		blockSizeX_ = width_ / blockSize_;
    blockSizeY_ = height_ / blockSize_;
		std::ifstream obj("../app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.obj");
		std::string line;
		std::stringstream ssObj;
		while(std::getline(obj, line)) {
			ssObj << line;
		}
		std::ifstream mtl("../app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.mtl");
		std::stringstream ssMtl;
		while(std::getline(mtl, line)) {
			ssMtl << line;
		}
		Components::OBJLoader objLoader{ssObj.str(), ssMtl.str()};
    objLoader.process();
		LOG("OBJ = ", ssObj.str());
		LOG("MTL = ", ssMtl.str());

		MobileRT::Scene scene_;
    samplesPixel_ = static_cast<unsigned int>(samplesPixel);
    samplesLight_ = static_cast<unsigned int>(samplesLight);
    LOG("samplesPixel_ = ", samplesPixel_);
    LOG("samplesLight_ = ", samplesLight_);
    switch (scene) {
        case 1:
            /*camera_ = new Components::Orthographic(MobileRT::Point3D(0.0f, 0.5f, 1.0f),
                                                   MobileRT::Point3D(0.0f, 0.0f, 7.0f),
                                                   MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                   6.5f, 4.5f);
            scene_ = spheresScene(std::move(scene_));*/
            objLoader.fillTriangles(scene_);
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.5f, 3.0f),
                                                  MobileRT::Point3D(0.0f, 0.5f, -1.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  45.0f, 45.0f * ratio);
            break;

        case 2:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.5f, 1.0f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 7.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  60.0f, 60.0f * ratio);
            scene_ = spheresScene2(std::move(scene_));
            break;

        case 3:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.0f, -3.4f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 1.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  45.0f, 45.0f * ratio);
            scene_ = cornellBoxScene2(std::move(scene_));
            break;

        default:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.0f, -3.4f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 1.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  45.0f, 45.0f * ratio);
            scene_ = cornellBoxScene(std::move(scene_));
            break;
    }
    switch (sampler) {
        case 1:
            if (samplesPixel_ > 1u) {
                samplerPixel_ = new Components::HaltonSeq(width_ * height_ * 2llu * samplesPixel_,
                                                          1u);
            } else {
                samplerPixel_ = new Components::Constant(0.5f);
            }
            samplerCamera_ = new Components::HaltonSeq(width_, height_, samplesPixel_,
                                                       blockSizeX_, blockSizeY_);
            break;

        default:
            if (samplesPixel_ > 1u) {
                samplerPixel_ = new Components::Stratified(width_ * height_ * 2llu * samplesPixel_,
                                                           1u);
            } else {
                samplerPixel_ = new Components::Constant(0.5f);
            }
            samplerCamera_ = new Components::Stratified(width_, height_, samplesPixel_,
                                                        blockSizeX_, blockSizeY_);
            break;
    }
    samplerRay_ = nullptr;
    samplerPointLight_ = nullptr;
    const uint64_t domainRay(
            (width_ * height_ * 2ull) * samplesPixel_ * RAY_DEPTH_MAX);
    const uint64_t domainLight(
            (width_ * height_ * 2ull) * samplesPixel_ * RAY_DEPTH_MAX * samplesLight_);
    switch (shader) {
        case 1:
            shader_ = new Components::Whitted(std::move(scene_), samplesLight_);
            break;

        case 2: LOG("domainRay = ", domainRay, "domainLight = ", domainLight);
            //samplerRay_ = new Components::HaltonSeq(domainRay, 1u);
            samplerRay_ = new Components::MersenneTwister(domainRay, 1u);
            //samplerLight_ = new Components::HaltonSeq(domainLight, 1);
            samplerLight_ = new Components::MersenneTwister(domainLight, 1u);
            samplerRussianRoulette_ = new Components::MersenneTwister(domainLight, 1);
            shader_ = new Components::PathTracer(
                    std::move(scene_), samplerRay_, samplerLight_,
                    samplerRussianRoulette_, samplesLight_);
            break;

        case 3:
            shader_ = new Components::DepthMap(std::move(scene_), MobileRT::Point3D(1, 1, 1));
            break;

        case 4:
          shader_ = new Components::DiffuseMaterial(std::move(scene_));
            break;

        default:
            shader_ = new Components::NoShadows(std::move(scene_), samplesLight_);
            break;
    }
    renderer_ = new MobileRT::Renderer(samplerCamera_, shader_, *camera_, width_,
                                       height_, blockSizeX_, blockSizeY_, samplerPixel_);

    LOG("x = ", blockSizeX_, "[", width_, "]");
    LOG("y = ", blockSizeY_, "[", height_, "]");
		LOG("TRIANGLES = ", shader_->scene_.triangles_.size());
		LOG("SPHERES = ", shader_->scene_.spheres_.size());
		LOG("PLANES = ", shader_->scene_.planes_.size());
		LOG("LIGHTS = ", shader_->scene_.lights_.size());
		LOG("MATERIALS = ", shader_->scene_.materials_.size());

    LOG("Threads = ", threads);
    const double start(omp_get_wtime());
		do {
			renderer_->renderFrame(bitmap, static_cast<unsigned int>(threads));
		camera_->position_.x_ += 2.0f;
		} while (repeats-- > 1);

		delete camera_;
		delete shader_;
		delete samplerCamera_;
		delete samplerPixel_;
		delete samplerRay_;
		delete samplerLight_;
    delete samplerRussianRoulette_;
		delete samplerPointLight_;
    delete renderer_;

    std::cout << "Time in secs = ";
		std::cout << omp_get_wtime() - start << std::endl;

    for (unsigned int i(0u), j(0u); i < width_ * height_ * 4u; i += 4u, j += 1u) {
        const unsigned int color(bitmap[j]);
        buffer[i + 0] = static_cast<unsigned char> ((color & 0x000000FF) >> 0);
        buffer[i + 1] = static_cast<unsigned char> ((color & 0x0000FF00) >> 8);
        buffer[i + 2] = static_cast<unsigned char> ((color & 0x00FF0000) >> 16);
        buffer[i + 3] = static_cast<unsigned char> ((color & 0xFF000000) >> 24);
    }
    gtk_init(&argc, &argv);
    GtkWidget *window (gtk_window_new(GTK_WINDOW_TOPLEVEL));
    GdkPixbuf *pixbuff (gdk_pixbuf_new_from_data(buffer, GDK_COLORSPACE_RGB, TRUE, 8,
    	static_cast<int> (width_), static_cast<int> (height_), static_cast<int> (width_ * 4), nullptr, nullptr));
    GtkWidget *image (gtk_image_new_from_pixbuf(pixbuff));
		gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(
			[]() -> void {
				delete[] bitmap;
				delete[] buffer;
				gtk_main_quit();
			}
		), nullptr);
		auto *check_escape (static_cast<bool(*)(
			GtkWidget* gtkWidget, GdkEventKey *event, gpointer /*unused*/)>(
			[](GtkWidget* gtkWidget, GdkEventKey *event, gpointer /*unused*/){
			if (event->keyval == GDK_KEY_Escape) {
					gtk_widget_destroy(gtkWidget);
					gtk_main_quit();
					return true;
				}
				return false;
			})
		);
    gtk_signal_connect(GTK_OBJECT(window), "key_press_event", GTK_SIGNAL_FUNC(check_escape), nullptr);
    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all(window);
    gtk_main();
		g_object_unref (G_OBJECT(pixbuff));
    return argc;
}
