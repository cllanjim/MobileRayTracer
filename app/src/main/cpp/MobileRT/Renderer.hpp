//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RENDERER_HPP
#define MOBILERAYTRACER_RENDERER_HPP

#include "Scene.hpp"
#include "RayTrace.hpp"
#include "RTCamera.hpp"

namespace MobileRT
{
    class Renderer
    {
        private:
            const unsigned int RT_W;
            const unsigned int RT_H;
            const unsigned int LowX;
            const unsigned int LowY;
            Scene* scene_;
            RayTrace* rTracer_;
            RTCamera* camera_;

        public:
            Renderer (const unsigned int pcanvasW, const unsigned int pcanvasH, const unsigned int renderRes, const unsigned int whichScene, const unsigned int whichShader);
            void render (unsigned int* canvas, const unsigned int width) const;
    };
}

#endif //MOBILERAYTRACER_RENDERER_HPP