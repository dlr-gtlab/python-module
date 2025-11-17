/* GTlab - Gas Turbine laboratory
 * Source File: httpresource.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt/resource/data/httpresource.h"

#include <QTimer>
#include <QEventLoop>
#include <QNetworkReply>

namespace gt
{

namespace resource
{

namespace data
{

HttpResource::HttpResource(const QUrl& url) :
    StreamResource(url)
{
    setObjectName(url.host());
    setFlag(GtObject::UserDeletable, true);
    setFlag(GtObject::UserRenamable, true);

    setUrlLocked(false);
}

bool
HttpResource::exists() const
{
    QNetworkRequest request{url()};
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    QNetworkAccessManager manager;
    QEventLoop loop;
    QTimer timer;

    QNetworkReply* reply = manager.head(request);

    connect(&manager, &QNetworkAccessManager::finished, &loop,
            &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(5000);

    loop.exec();

    bool ok = reply->error() == QNetworkReply::NoError &&
              reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200;

    reply->deleteLater();

    return ok;
}

QByteArray
HttpResource::readAll()
{
    return request(
        QNetworkRequest{url()}, QNetworkAccessManager::GetOperation);
}

std::unique_ptr<QIODevice>
HttpResource::open(QIODevice::OpenMode mode) const
{
    return nullptr;
}

QByteArray
HttpResource::request(
    const QNetworkRequest& request, QNetworkAccessManager::Operation op,
    const QByteArray& data)
{
    QNetworkAccessManager manager;
    QNetworkReply* reply = nullptr;

    switch (op)
    {
    case QNetworkAccessManager::GetOperation:
        reply = manager.get(request);
        break;
    default:
        return {};
    }

    QEventLoop loop{};
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    QByteArray result;
    if (reply->error() == QNetworkReply::NoError) result = reply->readAll();

    reply->deleteLater();
    return result;
}

} // namespace data

} // namespace resource

} // namespace gt
