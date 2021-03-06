//
// Created by Tiago on 15-Feb-17.
//

#ifndef APP_DRAWVIEW_HPP
#define APP_DRAWVIEW_HPP

#include "Scenes/Scenes.hpp"
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
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <memory>
#include <thread>

enum class State {
    IDLE = 0, BUSY = 1, FINISHED = 2, STOPPED = 3
};

extern "C"
jint JNI_OnLoad(JavaVM *pjvm, void *reserved);

extern "C"
void JNI_OnUnload(JavaVM *vm, void *reserved);

extern "C"
jobject Java_puscas_mobilertapp_DrawView_initVerticesArray(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
jobject Java_puscas_mobilertapp_DrawView_initColorsArray(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
jobject Java_puscas_mobilertapp_DrawView_initCameraArray(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
jint Java_puscas_mobilertapp_ViewText_isWorking(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_stopRender(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
jint Java_puscas_mobilertapp_DrawView_initialize(
        JNIEnv *env,
        jobject thiz,
        jint scene,
        jint shader,
        jint width,
        jint height,
        jint accelerator,
        jint samplesPixel,
        jint samplesLight,
        jstring objFile,
        jstring matFile,
        jobject assetManager
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_finishRender(
        JNIEnv */*env*/,
        jobject /*thiz*/
) noexcept;

extern "C"
void Java_puscas_mobilertapp_DrawView_renderIntoBitmap(
        JNIEnv *env,
        jobject thiz,
        jobject dstBitmap,
        jint nThreads,
        jboolean async) noexcept;

extern "C"
jint Java_puscas_mobilertapp_DrawView_traceTouch(
        JNIEnv *env,
        jobject thiz,
        jfloat jx,
        jfloat jy) noexcept;

extern "C"
void Java_puscas_mobilertapp_ViewText_moveTouch(
        JNIEnv *env,
        jobject thiz,
        jfloat jx,
        jfloat jy,
        jint primitiveIndex
) noexcept;

extern "C"
jfloat Java_puscas_mobilertapp_ViewText_getFPS(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
jint Java_puscas_mobilertapp_ViewText_getTimeFrame(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
jint Java_puscas_mobilertapp_ViewText_getSample(
        JNIEnv *env,
        jobject thiz
) noexcept;

extern "C"
jint Java_puscas_mobilertapp_DrawView_resize(
        JNIEnv *env,
        jobject thiz,
        jint size
) noexcept;

extern "C"
jint Java_puscas_mobilertapp_DrawView_getNumberOfLights(
        JNIEnv * /*env*/,
        jobject /*thiz*/
) noexcept;

extern "C"
jint Java_puscas_mobilertapp_DrawView_getNumberOfTriangles(
        JNIEnv * /*env*/,
        jobject /*thiz*/
) noexcept;

extern "C"
jobject Java_puscas_mobilertapp_DrawView_freeNativeBuffer(
        JNIEnv *env, jobject thiz, jobject bufferRef) noexcept;

#endif //APP_DRAWVIEW_HPP
