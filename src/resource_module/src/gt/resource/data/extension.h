/* GTlab - Gas Turbine laboratory
 * Source File: extension.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef EXTENSION_H
#define EXTENSION_H

#include <memory>
#include <vector>
#include <functional>

#include <QMetaObject>

class GtObject;

namespace gt
{

namespace resource
{

namespace data
{

namespace extension
{

using ChildFactoryFunc = std::function<std::unique_ptr<GtObject>()>;

void registerChildFactory(
    const QMetaObject& parentMetaObj, ChildFactoryFunc func);

const std::vector<ChildFactoryFunc>& childFactories(
    const QMetaObject& parentMetaObj);

void registerChildFactoryForPackage(ChildFactoryFunc func);

void registerChildFactoryForScripts(ChildFactoryFunc func);

template<typename ParentType>
const std::vector<ChildFactoryFunc>& childFactories()
{
    static_assert(std::is_base_of<GtObject, ParentType>::value,
                  "ParentType must derive from GtObject");

    return childFactories(ParentType::staticMetaObject);
}

template<typename ParentType, typename ChildType>
void registerChild()
{
    static_assert(std::is_base_of<GtObject, ChildType>::value,
                  "ChildType must derive from GtObject");
    static_assert(std::is_base_of<GtObject, ParentType>::value,
                  "ParentType must derive from GtObject");

    registerChildFactory(ParentType::staticMetaObject, []() {
        return std::make_unique<ChildType>();
    });
}

template<typename ChildType>
void registerChildForPackage()
{
    static_assert(std::is_base_of<GtObject, ChildType>::value,
                  "ChildType must derive from GtObject");

    registerChildFactoryForPackage([]() {
        return std::make_unique<ChildType>();
    });
}

template<typename ChildType>
void registerChildForScripts()
{
    static_assert(std::is_base_of<GtObject, ChildType>::value,
                  "ChildType must derive from GtObject");

    registerChildFactoryForScripts([]() {
        return std::make_unique<ChildType>();
    });
}

template<typename ParentType>
void appendDefaultChildren(ParentType* parent)
{
    static_assert(std::is_base_of<GtObject, ParentType>::value,
                  "ParentType must derive from GtObject");
    if (!parent) return;

    for (auto& createObj : childFactories<ParentType>())
    {
        auto obj = createObj();
        if (!obj) continue;

        obj->setDefault(true);
        if (parent->appendChild(obj.get())) obj.release();
    }
}

} // namespace extension

} // namespace data

} // namespace resource

} // namespace gt

#endif // EXTENSION_H
