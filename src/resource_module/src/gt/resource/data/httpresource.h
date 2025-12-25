/* GTlab - Gas Turbine laboratory
 * Source File: httpresource.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef HTTPRESOURCE_H
#define HTTPRESOURCE_H

#include <QNetworkAccessManager>

#include "gt/resource/data/streamresource.h"

namespace gt
{

namespace resource
{

namespace data
{

class HttpResource : public StreamResource
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit HttpResource(const QUrl& url = {});

    ~HttpResource() override = default;

    bool exists() const override;

    QByteArray readAll() override;

protected:
    std::unique_ptr<QIODevice> open(QIODevice::OpenMode mode) const override;

private:
    static QByteArray request(const QNetworkRequest& request,
                              QNetworkAccessManager::Operation op,
                              const QByteArray& data = {});
};

} // namespace data

} // namespace resource

} // namespace gt

#endif // HTTPRESOURCE_H
