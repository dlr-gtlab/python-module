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

#include <gt_project.h>

namespace gt
{

namespace resource
{

namespace url
{

namespace helper
{

/**
 * @brief Returns the absolute, resolved path to the project root.
 * Any symlinks in the project directory are resolved.
 *
 * @param proj Reference to the GtProject object.
 * @return QString Canonical path of the project directory.
 */
inline QString projRoot(const GtProject& proj)
{
    return QDir{proj.path()}.canonicalPath();
}

} // namespace helper

/**
 * @brief Returns the fully encoded string representation of a QUrl.
 * @param url The QUrl to encode.
 * @return QString Fully encoded URL string.
 */
inline QString fullyEncoded(const QUrl& url)
{
    return url.toString(QUrl::FullyEncoded);
}

/**
 * @brief Converts an absolute local file path to a file:// URL.
 * @param path Absolute path to a file or directory.
 * @return QUrl Corresponding file:// URL.
 */
inline QUrl fromAbsPath(const QString& path)
{
    return QUrl::fromLocalFile(QDir::cleanPath(path));
}

/**
 * @brief Creates a proj:// URL from a project-relative path.
 * The path is normalized, cleaned, and prepended with a leading slash.
 * @param relPath Path relative to the project root.
 * @return QUrl Corresponding proj:// URL, or {} if the path is empty.
 */
inline QUrl fromProjRelPath(const QString& relPath)
{
    QString path = QDir::cleanPath(relPath);

    if (path.isEmpty()) return {};
    if (!path.startsWith("/")) path.prepend("/");

    QUrl url{};
    url.setScheme("proj");
    url.setPath(path);

    return url;
}

/**
 * @brief Converts an absolute file:// URL to a proj:// URL. It checks whether
 * the file or directory is inside the project root. If the file does not
 * exist, it still returns a valid proj:// URL.
 * @param proj Pointer to the project that the file belongs to.
 * @param absUrl Absolute file:// URL of the file or directory.
 * @return QUrl Corresponding proj:// URL, or {} if the path is invalid.
 */
inline QUrl toProjFileUrl(const GtProject* proj, const QUrl& absUrl)
{
    if (!proj || absUrl.scheme() != "file") return {};

    // get project root path
    const QString projRoot = helper::projRoot(*proj);
    if (projRoot.isEmpty()) return {};

    // convert absUrl to a clean absolute filesystem path
    QString absPath = QDir::cleanPath(absUrl.toLocalFile());
    absPath = QFileInfo{absPath}.absoluteFilePath();

    // ensure the file or directory is located inside the project root
    if (!absPath.startsWith(QDir::cleanPath(projRoot + QDir::separator())))
    {
        return {};
    }

    // determine the path relative to the project root
    QString relPath = QDir{projRoot}.relativeFilePath(absPath);
    relPath = QDir::cleanPath(relPath);

    if (relPath.startsWith("..") || relPath.isEmpty()) return {};

    // convert the relative path to a proj:// URL
    return fromProjRelPath(relPath);
}

/**
 * @brief Converts an absolute file path string to a proj:// URL.
 * It is a convenience wrapper that first converts the absolute path
 * to a file:// URL and then calls the QUrl version of toProjFileUrl().
 * @param proj Pointer to the project.
 * @param absPath Absolute path to a file or directory.
 * @return QUrl Corresponding proj:// URL, or {} if the path is invalid.
 */
inline QUrl toProjFileUrl(const GtProject* proj, const QString& absPath)
{
    return toProjFileUrl(proj, fromAbsPath(absPath));
}

/**
 * @brief Converts a proj:// URL back to an absolute file:// URL.
 * The URL must start with proj:// and contain a valid relative path.
 * @param proj Pointer to the project.
 * @param projUrl The proj:// URL of the file or directory.
 * @return QUrl Corresponding file:// URL, or {} if the URL is invalid.
 */
inline QUrl toAbsFileUrl(const GtProject* proj, const QUrl& projUrl)
{
    if (!proj || projUrl.scheme() != "proj") return {};

    QString path = projUrl.path();
    if (!path.startsWith("/")) return {};

    path = QDir::cleanPath(path.mid(1));

    const QString projRoot = helper::projRoot(*proj);
    if (projRoot.isEmpty()) return {};

    return fromAbsPath(QDir{projRoot}.absoluteFilePath(path));
}

/**
 * @brief Converts a proj:// URL to an absolute file system path.
 * @param proj Pointer to the project.
 * @param projUrl The proj:// URL.
 * @return QString Absolute path to the file or directory; empty if the URL is
 * invalid.
 */
inline QString toAbsPath(const GtProject* proj, const QUrl& projUrl)
{
    return toAbsFileUrl(proj, projUrl).toLocalFile();
}

} // namespace url

} // namespace resource

} // namespace gt

#endif // URL_H
