//
// Created by Tiago on 15-Feb-17.
//

#ifndef APP_DRAWVIEW_HPP
#define APP_DRAWVIEW_HPP

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
#include "MobileRT/Shapes/Plane.hpp"
#include "MobileRT/Shapes/Rectangle.hpp"
#include "MobileRT/Shapes/Sphere.hpp"
#include "MobileRT/Shapes/Triangle.hpp"
#include "Scenes.hpp"
#include <android/bitmap.h>
#include <android/log.h>
#include <memory>
#include <thread>

enum class State {
  IDLE = 0, BUSY = 1, FINISHED = 2, STOPPED = 3
};

static void FPS() noexcept;

static void thread_work(void *dstPixels, int numThreads) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_finish(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
State Java_puscas_mobilertapp_DrawView_isWorking (
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_stopRender(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_initialize(
        JNIEnv *env,
        jobject thiz,
        jint scene,
        jint shader,
        jint width,
        jint height,
        jint sampler,
        jint samplesPixel,
        jint samplesLight,
        jstring objFile,
        jstring matText
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_renderIntoBitmap(
        JNIEnv *env,
        jobject thiz,
        jobject dstBitmap,
        jint nThreads) noexcept;

extern "C"
int Java_puscas_mobilertapp_DrawView_traceTouch(
        JNIEnv *env,
        jobject thiz,
        jfloat jx,
        jfloat jy) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_moveTouch(
        JNIEnv *env,
        jobject thiz,
        jfloat jx,
        jfloat jy,
        jint primitiveIndex
) noexcept;

extern "C"
float Java_puscas_mobilertapp_DrawView_getFPS(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
int64_t Java_puscas_mobilertapp_DrawView_getTimeFrame(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
unsigned Java_puscas_mobilertapp_DrawView_getSample (
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
int Java_puscas_mobilertapp_DrawView_resize(
        JNIEnv *env,
        jobject thiz,
        jint size
) noexcept;

#endif //APP_DRAWVIEW_HPP
