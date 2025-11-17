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

#include "gt/resource/url.h"

namespace gt
{

namespace resource
{

namespace data
{

gt::resource::data::File::File(const QUrl& url) : StreamResource(url)
{
    setObjectName(url.fileName());
    setFlag(GtObject::UserRenamable, true);
    setFlag(GtObject::UserDeletable, true);
}

File::File(const QString &localPath) :
    File(gt::resource::url::fromLocalFile(localPath)) { }

File::~File() = default;

bool File::exists() const
{
    QFileInfo info{url().toLocalFile()};
    return info.exists() && info.isFile();
}

QString File::path() const { return url().toLocalFile(); }

std::unique_ptr<QIODevice> File::open(QIODevice::OpenMode mode) const
{
    auto file = std::make_unique<QFile>(url().toLocalFile());
    if (!file->open(mode)) return nullptr;

    return file;
}


} // namespace data

} // namespace resource

} // namespace gt
