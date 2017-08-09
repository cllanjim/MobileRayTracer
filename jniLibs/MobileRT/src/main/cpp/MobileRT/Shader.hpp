//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHADER_HPP
#define MOBILERT_SHADER_HPP

#include "Accelerators/RegularGrid.hpp"
#include "Intersection.hpp"
#include "RGB.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vector3D.hpp"
#include <utility>

namespace MobileRT {
    class Shader {
    public:
		Scene scene_;
		RegularGrid regularGrid_;

    protected:
      const unsigned int samplesLight_ {};

    protected:
			virtual bool shade (RGB *rgb, Intersection intersection,
													Ray &&ray) const noexcept = 0;

      private:
      template<typename T>
      void getSceneBounds(std::vector<T> primitives, Point3D *min, Point3D *max);

		public:
			explicit Shader(Scene &&scene, unsigned int samplesLight = 0) noexcept;

			Shader(const Shader &shader) noexcept = delete;

			Shader(Shader &&shader) noexcept = delete;

			virtual ~Shader() noexcept;

			Shader &operator=(const Shader &shader) noexcept = delete;

			Shader &operator=(Shader &&shader) noexcept = delete;

			//ray trace and verifies if intersects primitives
			bool rayTrace(RGB *rgb, Intersection *intersection, Ray &&ray) const noexcept;

      int traceTouch(Intersection *intersection, Ray &&ray) const noexcept;

			virtual void resetSampling() noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERT_SHADER_HPP
