//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERT_UTILS_HPP
#define MOBILERT_UTILS_HPP

#include <csignal>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#ifndef NO_ANDROID

#include <android/log.h>

#endif

template <typename T>
void MagicLog(std::ostream& o, T t)
{
    o << t;
}

template<typename T, typename... Args>
void MagicLog(std::ostream& o, T t, Args... args)
{
    MagicLog(o << t, args...);
}

template<typename... Args>
void log(Args... args)
{
    std::ostringstream oss("");
    MagicLog(oss, args...);
	#ifdef NO_ANDROID
		std::cout << oss.str() << std::endl;
	#else
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%s\n", oss.str().c_str());
	#endif
}

#define LOG(...) {log(getFileName(__FILE__),":",__LINE__,": ",__VA_ARGS__);}

#ifndef LOG
	#define LOG(...)
#endif

inline std::string getFileName (const char *filepath) {
	std::stringstream ss (filepath);
	std::string filePath(ss.str());
	std::string::size_type filePos (filePath.rfind('/'));
	if (filePos != std::string::npos) {
		++filePos;
	}
	else {
		filePos = 0;
	}
	return std::string(filePath.substr(filePos));
}

#define RAY_LENGTH_MIN  1.0e-3f
#define RAY_LENGTH_MAX  1.0e+5f
#define RAY_DEPTH_MIN   4u
#define RAY_DEPTH_MAX   8u
#define PI              3.14159265358979323846f
#define PI_4            0.78539816339744830962f
#define VECT_PROJ_MIN   1.0e-3f // kEpsilon

unsigned int roundDownToMultipleOf(unsigned int value,
                                   unsigned int multiple) noexcept;

#endif //MOBILERT_UTILS_HPP
