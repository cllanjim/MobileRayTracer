//
// Created by puscas on 30/07/17.
//

#include "ObjectLoader.hpp"

using MobileRT::ObjectLoader;

ObjectLoader::~ObjectLoader() noexcept {
    LOG("ObjectLoader DELETED");
}

bool ObjectLoader::isProcessed() const {
    return isProcessed_;
}