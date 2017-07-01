//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADER_H
#define MOBILERAYTRACER_SHADER_H

#include "Scene.h"
#include "RGB.h"
#include "Intersection.h"
#include "Ray.h"
#include "Vector3D.h"

namespace MobileRT {
    class Shader {
		public:
			Scene &scene_;

		protected:
			const unsigned int samplesLight_;

		protected:
			virtual void shade(RGB &, const Intersection &, const Ray &) const = 0;

		public:
			explicit Shader(Scene &scene, const unsigned int samplesLight);

			virtual ~Shader(void);

			void rayTrace(RGB &rgb, const Ray &ray, Intersection &intersection) const;

			int traceTouch(Intersection &intersection, const Ray &ray) const;

			virtual void resetSampling(void) = 0;
    };
}

#endif //MOBILERAYTRACER_SHADER_H
