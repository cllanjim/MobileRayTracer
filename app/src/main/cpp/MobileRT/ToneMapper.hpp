//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_TONEMAPPER_HPP
#define MOBILERAYTRACER_TONEMAPPER_HPP

#include "RGB.hpp"

namespace MobileRT
{
    class ToneMapper
    {
        private:
            

        public:
            static unsigned int RGB2Color (const RGB& px);
    };
}

#endif //MOBILERAYTRACER_TONEMAPPER_HPP