/* GTlab - Gas Turbine laboratory
 * Source File: file.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt/resource/data/file.h"

#include <QFileInfo>

#include "gt/resource/url.h"

namespace gt
{

namespace resource
{

namespace data
{

File::File(const QUrl& url) : StreamResource(url)
{
    setFlag(GtObject::UserDeletable, true);

    setObjectName(url.fileName());
}

File::File(const QString& localPath) :
    File(gt::resource::url::fromAbsPath(localPath)) { }

File::~File() = default;

bool
File::exists() const
{
    QFileInfo fi = info();
    return fi.exists() && fi.isFile();
}

QString File::path() const { return toPath(url()); }

QFileInfo
File::info() const
{
    return {path()};
}

std::unique_ptr<QIODevice>
File::open(QIODevice::OpenMode mode) const
{
    auto file = std::make_unique<QFile>(path());
    if (!file->open(mode)) return nullptr;

    return file;
}

QString
File::toPath(const QUrl& url) const
{
    return QDir::cleanPath(url.toLocalFile());
}

} // namespace data

} // namespace resource

} // namespace gt
