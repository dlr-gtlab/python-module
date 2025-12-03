/* GTlab - Gas Turbine laboratory
 * Source File: streamresource.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef STREAMRESOURCE_H
#define STREAMRESOURCE_H

#include <QIODevice>

#include "gt/resource/data/urlresource.h"

namespace gt
{

namespace resource
{

namespace data
{

class StreamResource : public UrlResource
{
    Q_OBJECT

public:
    explicit StreamResource(const QUrl& url);

    ~StreamResource() override;

    virtual QByteArray readAll();

    virtual bool writeAll(const QByteArray&);

    virtual bool exists() const = 0;

protected:
    virtual std::unique_ptr<QIODevice> open(QIODevice::OpenMode mode) const = 0;
};

} // namespace data

} // namespace resource

} // namespace gt

#endif // STREAMRESOURCE_H
