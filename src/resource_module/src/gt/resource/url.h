/* GTlab - Gas Turbine laboratory
 * Source File: url.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef URL_H
#define URL_H

#include <QUrl>
#include <QDir>

#include "gt_project.h"

namespace gt
{

namespace resource
{

namespace url
{

inline QString fullyEncoded(const QUrl& url)
{
    return url.toString(QUrl::FullyEncoded);
}

inline QUrl fromLocalFile(const QString& path)
{
    return QUrl::fromLocalFile(path);
}

inline QUrl fromProjFile(const QString& relPath)
{
    QUrl url{};
    url.setScheme("proj");
    url.setPath(QDir::cleanPath(relPath));
    return url;
}

inline QUrl toAbsoluteFileUrl(const GtProject* proj, const QUrl& projUrl)
{
    if (!proj || projUrl.scheme() != "proj") return projUrl;

    return fromLocalFile(QDir{proj->path()}.absoluteFilePath(projUrl.path()));
}

inline QUrl toProjFileUrl(const GtProject* proj, const QString& path)
{
    auto absUrl = fromLocalFile(path);
    if (!proj || absUrl.scheme() != "file") return absUrl;

    const QString absPath = absUrl.toLocalFile();
    QDir projDir{proj->path()};
    QString relPath = projDir.relativeFilePath(absPath);

    if (relPath.startsWith("..")) return absUrl;

    return fromProjFile(relPath);
}

}// namespace url

} // namespace resource

} // namespace gt

#endif // URL_H
