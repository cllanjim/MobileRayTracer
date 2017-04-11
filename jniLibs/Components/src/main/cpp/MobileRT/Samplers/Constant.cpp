//
// Created by puscas on 07-04-2017.
//

#include "Constant.h"

using namespace MobileRT;

Constant::Constant(const float value) :
        Sampler(0, 0),
        value_(value)
{
}

float Constant::getDeviation(const unsigned int)
{
    return value_;
}

float Constant::getSample(const unsigned int)
{
    return value_;
}
