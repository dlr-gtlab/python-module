/* GTlab - Gas Turbine laboratory
 * Source File: directory.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt/resource/data/directory.h"

#include <QFileInfo>

#include <gt_datamodel.h>
#include <gt_boolproperty.h>

#include "gt/resource/url.h"
#include "gt/resource/data/file.h"

namespace gt
{

namespace resource
{

namespace data
{

struct Directory::Impl
{
    Impl()
    {
        isOpen = false;
        isOpen.hide();
    }

    GtBoolProperty isOpen{"open", "open"};
    QStringList fileFilters{"*"};
};

Directory::Directory(const QUrl& url) : UrlResource(url),
    m_pimpl(std::make_unique<Impl>())
{
    setFlag(GtObject::UserRenamable, true);
    setFlag(GtObject::UserDeletable, true);

    registerProperty(m_pimpl->isOpen);
}

Directory::Directory(const QString &localPath) :
    Directory(gt::resource::url::fromLocalFile(localPath)) { }

Directory::~Directory() = default;

bool
Directory::exists() const
{
    QFileInfo info{url().toLocalFile()};
    return info.exists() && info.isDir();
}

bool
Directory::open()
{
    if (!exists()) return false;

    refresh();
    m_pimpl->isOpen = true;

    return true;
}

void
Directory::close()
{
    if (!m_pimpl->isOpen) return;

    clearChildren();
    m_pimpl->isOpen = false;
}

bool
Directory::isOpen() const
{
    return m_pimpl->isOpen;
}

void
Directory::refresh()
{
    clearChildren();

    QDir dir{url().toLocalFile()};
    auto filters = fileFilters();
    dir.setNameFilters(filters.isEmpty() ? QStringList{"*"} : filters);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    const auto& fileInfos = dir.entryInfoList();
    for (const QFileInfo& fileInfo : fileInfos)
    {
        if (auto fileObj = createFileResource(fileInfo))
        {
            gtDataModel->appendChild(fileObj.get(), this);
            if (fileObj->parent() == this) fileObj.release();
        }
    }
}

std::unique_ptr<File>
Directory::createFileResource(const QFileInfo& file)
{
    return std::make_unique<File>(
        gt::resource::url::fromLocalFile(file.absoluteFilePath()));
}

const QStringList&
Directory::fileFilters() const
{
    return m_pimpl->fileFilters;
}

void
Directory::setFileFilters(const QStringList& fileFilters)
{
    m_pimpl->fileFilters = fileFilters;
}

void
Directory::clearChildren()
{
    const auto& files = findDirectChildren<File*>();
    for (auto* file : files)
    {
        gtDataModel->deleteFromModel(file);
    }
}

} // namespace data

} // namespace resource

} // namespace gt
