#ifndef C_WRAPPER_HPP
#define C_WRAPPER_HPP

#include <cstdint>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
#endif
void RayTrace(::std::uint32_t *bitmap, ::std::int32_t width, ::std::int32_t height, ::std::int32_t threads, ::std::int32_t shader, ::std::int32_t scene,
              ::std::int32_t samplesPixel, ::std::int32_t samplesLight, ::std::int32_t repeats, ::std::int32_t accelerator, bool printStdOut,
              bool async, const char *pathObj, const char *pathMtl);

#endif // C_WRAPPER_HPP
