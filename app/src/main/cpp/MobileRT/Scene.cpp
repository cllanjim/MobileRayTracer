//
// Created by puscas on 16-10-2016.
//

#include "Scene.hpp"
#include "Constants.hpp"

using namespace MobileRT;

Scene::Scene()
{
}

Scene::~Scene()
{
    for (unsigned i = 0; i < this->lights.size(); i++)
    {
        delete this->lights[i];
    }
    
    for (unsigned i = 0; i < this->primitives.size(); i++)
    {
        delete this->primitives[i];
    }
}

bool Scene::trace(const Ray& r, Intersection& intersection) const//TODO: utilizar estrutura de aceleraçao
{
    bool value = false;
    float minT = MAX_LENGTH;
    const unsigned int n = this->primitives.size();

    for (unsigned int i = 0; i < n; i++)//nao e preciso tar sempre a verificar todas as primitivas
    {
        if (this->primitives[i]->Intersect(r, minT, intersection))
        {
            minT = intersection.length();
            value = true;
        }
    }
    return value;
}

bool Scene::shadowTrace(const Ray& ray, Intersection& intersection) const//TODO: utilizar estrutura de aceleraçao
{
    const unsigned int  n = this->primitives.size();
    for (unsigned int i = 0; i < n; i++)//fazer trace do shadow ray
    {
        if (this->primitives[i]->Intersect(ray, intersection))
        {
            return true;
        }
    }
    return false;
}