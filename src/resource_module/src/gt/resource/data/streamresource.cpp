/* GTlab - Gas Turbine laboratory
 * Source File: streamresource.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt/resource/data/streamresource.h"

namespace gt
{

namespace resource
{

namespace data
{

StreamResource::StreamResource(const QUrl& url) : UrlResource(url) { }

StreamResource::~StreamResource() = default;

QByteArray
StreamResource::readAll()
{
    auto device = open(QIODevice::ReadOnly);
    if (!device) return {};

    return std::move(device->readAll());
}

bool
StreamResource::writeAll(const QByteArray&)
{
    return false;
}

} // namespace data

} // namespace resource

} // namespace gt
