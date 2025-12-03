/* GTlab - Gas Turbine laboratory
 * Source File: extension.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "extension.h"

#include <mutex>
#include <unordered_map>

#include "gt/resource/data/package.h"
#include "gt/resource/data/scripts.h"


namespace gt
{

namespace resource
{

namespace data
{

namespace extension
{

namespace
{

using RegistryMap = std::unordered_map<
    const QMetaObject*, std::vector<ChildFactoryFunc>>;

RegistryMap& registry()
{
    static RegistryMap instance;
    return instance;
}

std::mutex& registryMutex()
{
    static std::mutex m;
    return m;
}

} // namespace


void registerChildFactory(
    const QMetaObject& parentMetaObj, ChildFactoryFunc func)
{
    std::lock_guard<std::mutex> lock(registryMutex());
    registry()[&parentMetaObj].push_back(std::move(func));
}

const std::vector<ChildFactoryFunc>& childFactories(
    const QMetaObject& parentMetaObj)
{
    std::lock_guard<std::mutex> lock(registryMutex());

    static const std::vector<ChildFactoryFunc> empty;
    auto it = registry().find(&parentMetaObj);

    return (it != registry().end()) ? it->second : empty;
}

void registerChildFactoryForPackage(ChildFactoryFunc func)
{
    registerChildFactory(gt::resource::data::Package::staticMetaObject, func);
}

void registerChildFactoryForScripts(ChildFactoryFunc func)
{
    registerChildFactory(gt::resource::data::Scripts::staticMetaObject, func);
}

} // namespace extension

} // namespace data

} // namespace resource

} // namespace gt
