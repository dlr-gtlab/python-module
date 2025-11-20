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
#include <QFileSystemWatcher>
#include <QRegularExpressionValidator>

#include <QTimer>
#include <QThread>
#include <QCoreApplication>

#include <gt_datamodel.h>
#include <gt_application.h>
#include <gt_boolproperty.h>
#include <gt_variantproperty.h>

#include "gt/resource/url.h"
#include "gt/resource/data/file.h"

#include "gt/resource/data/helper.h"

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

        fileFilters.setVal("*");
    }

    GtBoolProperty isOpen{"open", "open"};
    GtStringProperty fileFilters{"fileFilters", "fileFilters", "", "",
        new QRegularExpressionValidator{QRegularExpression{".*"}}};
    std::unique_ptr<QFileSystemWatcher> watcher{nullptr};
};

Directory::Directory(const QUrl& url) :
    UrlResource(url), m_pimpl(std::make_unique<Impl>())
{
    setFlag(GtObject::UserRenamable, true);
    setFlag(GtObject::UserDeletable, true);

    setObjectName(QFileInfo{url.toLocalFile()}.fileName());

    registerSilentProperty(m_pimpl->isOpen);
    registerProperty(m_pimpl->fileFilters);

    // This connection ensures that directories which were open when the project
    // was closed will be reopened when the project is opened again.
    connect(gtApp, &GtCoreApplication::currentProjectChanged, this,
            [this](GtProject* proj) {
                assert(proj == helper::findProject(uuid()));
                if (isOpen()) open();
            });

    connect(&m_pimpl->fileFilters, &GtVariantProperty::changed, this,
            [this](){ if (isOpen() && exists()) refresh(); });
}

Directory::Directory(const QString& localPath) :
    Directory(gt::resource::url::fromAbsPath(localPath)) { }

Directory::~Directory() = default;

bool
Directory::exists() const
{
    QFileInfo fi = info();
    return fi.exists() && fi.isDir();
}

bool
Directory::isOpen() const
{
    return m_pimpl->isOpen;
}

bool
Directory::open()
{
    if (!exists()) return false;

    m_pimpl->isOpen = true;

    initFileWatcher();
    refresh();

    return true;
}

void
Directory::close()
{
    if (!m_pimpl->isOpen) return;

    m_pimpl->isOpen = false;
    m_pimpl->watcher = nullptr;

    clearFileList();
}

void
Directory::clean()
{
    const auto fileObjs = fileList();
    for (auto* file : fileObjs)
    {
        if (!file->exists()) gtDataModel->deleteFromModel(file);
    }
}

QFileInfo
Directory::info() const
{
    return {path()};
}

QString
Directory::path() const
{
    return toPath(url());
}

void
Directory::refresh()
{
    QDir dir{path()};
    auto filters = fileFilters();
    dir.setNameFilters(filters.isEmpty() ? QStringList{"*"} : filters);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name | QDir::IgnoreCase);

    const auto fileObjs = fileList();
    const auto fileInfos = dir.entryInfoList();

    // collect allowed paths from the directory according to the current filters
    QSet<QString> allowedPaths{};
    for (const auto& fi : fileInfos) allowedPaths.insert(fi.absoluteFilePath());

    // check existing File objects: keep if allowed, otherwise remove from model
    QSet<QString> fileObjPaths{};
    for (auto* f : fileObjs)
    {
        auto filePath = f->path();
        if (allowedPaths.contains(filePath)) fileObjPaths.insert(filePath);
        else gtDataModel->deleteFromModel(f);
    }

    // create File objects for new files that are allowed but not yet
    // represented
    for (int i = 0; i < fileInfos.size(); ++i)
    {
        const QFileInfo& fi = fileInfos[i];
        if (fileObjPaths.contains(fi.absoluteFilePath())) continue;

        if (auto fileObj = createFileResource(fi))
        {
            gtDataModel->insertChild(fileObj.get(), this, i);
            if (fileObj->parent() == this) fileObj.release();
        }
    }
}

void
Directory::setIsOpen(bool isOpen)
{
    m_pimpl->isOpen = isOpen;
}

std::unique_ptr<File>
Directory::createFileResource(const QFileInfo& file)
{
    return std::make_unique<File>(
        gt::resource::url::fromAbsPath(file.absoluteFilePath()));
}

QString
Directory::toPath(const QUrl& url) const
{
    return QDir::cleanPath(url.toLocalFile());
}

QStringList
Directory::fileFilters() const
{
    return m_pimpl->fileFilters.get().split(";");
}

void
Directory::setFileFilters(const QStringList& fileFilters)
{
    m_pimpl->fileFilters.setVal(fileFilters.join(";"));
}

void
Directory::hideFileFiltersProperty(bool hide)
{
    m_pimpl->fileFilters.setReadOnly(hide);
    m_pimpl->fileFilters.hide(hide);
}

void
Directory::clearFileList()
{
    const auto& files = fileList();
    for (auto* file : files)
    {
        gtDataModel->deleteFromModel(file);
    }
}

void
Directory::initFileWatcher()
{
    m_pimpl->watcher = std::make_unique<QFileSystemWatcher>();
    connect(m_pimpl->watcher.get(), &QFileSystemWatcher::directoryChanged, this,
            [this](const QString&) {
                if (exists() && isOpen()) refresh();
            });
    m_pimpl->watcher->addPath(path());
}

QList<File*>
Directory::fileList()
{
    return findDirectChildren<File*>();
}

} // namespace data

} // namespace resource

} // namespace gt
