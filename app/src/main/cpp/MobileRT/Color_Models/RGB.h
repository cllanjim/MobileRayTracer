//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RGB_H
#define MOBILERAYTRACER_RGB_H

#include <mutex>

namespace MobileRT
{
    class RGB
    {
        private:
        std::mutex mutex;

        public:
            float R_;
            float G_;
            float B_;

            RGB ();
            RGB (const float r, const float g, const float b);
            RGB (const RGB& rgb);

            bool isZero() const;
            void add (const RGB& rgb);
            void mult (const RGB& rgb);
            void mult (const float f);

        void average(const RGB &rgb, const unsigned int number);

            void recycle ();
            void recycle (const float r, const float g, const float b);
    };
}

#endif //MOBILERAYTRACER_RGB_H