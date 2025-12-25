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

#include <QTimer>
#include <QThread>
#include <QCollator>
#include <QFileInfo>
#include <QCoreApplication>
#include <QFileSystemWatcher>
#include <QRegularExpressionValidator>

#include <gt_version.h>
#include <gt_datamodel.h>
#include <gt_application.h>
#include <gt_boolproperty.h>
#include <gt_propertyitem.h>
#include <gt_variantproperty.h>

#include "gt/resource/url.h"
#include "gt/resource/data/file.h"

#include "gt/resource/data/helper.h"


namespace
{

inline auto createRegExp(const QString& regExpStr)
{
#if GT_VERSION >= GT_VERSION_CHECK(2, 1, 0)
    return QRegularExpression{regExpStr};
#else
    return new QRegularExpressionValidator{QRegularExpression{regExpStr}};
#endif
}

bool fileMatchesFilter(const QString& filePath, const QStringList& filters)
{
    if (filters.isEmpty()) return true;

    const QString fileName = QFileInfo(filePath).fileName();

    for (const auto& filter  : filters)
    {
        QString reStr = QRegularExpression::wildcardToRegularExpression(filter);
        QRegularExpression re{reStr, QRegularExpression::CaseInsensitiveOption};
        if (re.match(fileName).hasMatch()) return true;
    }

    return false;
}

}

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
        isOpen.hide(true);
        isOpen.setReadOnly(true);

        isRefreshRequired = false;
        isRefreshRequired.hide(true);
        isRefreshRequired.setReadOnly(true);
    }

    GtBoolProperty isOpen{"open", "open"};

    GtBoolProperty isRefreshRequired{"refreshRequired", "refreshRequired"};

    GtStringProperty fileFilters{
        "fileFilters", tr("File Filters"), "", "*", createRegExp(".*")
    };

    GtBoolProperty deleteFilteredOutFiles{
        "deleteFilteredOutFiles", tr("Delete Filtered-out Files"),
        tr("If enabled, filtered-out file objects are deleted from the model. "
           "Otherwise, they are simply hidden."),
        false
    };

    GtBoolProperty deleteMissingFiles{
        "deleteMissingFiles", tr("Delete Missing Files"),
        tr("If enabled, file objects whose underlying files no longer exist on "
           "disk are removed from the model. If disabled, such objects are "
           "retained and marked as missing."),
        true
    };

    QString lastFileFilters{"*"};

    std::unique_ptr<QFileSystemWatcher> watcher{nullptr};
    std::unique_ptr<QTimer> watcherTimer{};
};

Directory::Directory(const QUrl& url) :
    UrlResource(url), m_pimpl(std::make_unique<Impl>())
{
    setFlag(GtObject::UserRenamable, true);
    setFlag(GtObject::UserDeletable, true);

    setObjectName(QFileInfo{url.toLocalFile()}.fileName());

    registerSilentProperty(m_pimpl->isOpen);
    registerSilentProperty(m_pimpl->isRefreshRequired);
    registerProperty(m_pimpl->fileFilters);
    registerProperty(m_pimpl->deleteFilteredOutFiles);
    registerProperty(m_pimpl->deleteMissingFiles);

    // This connection ensures that directories which were open when the project
    // was closed will be reopened when the project is opened again.
    connect(gtApp, &GtCoreApplication::currentProjectChanged, this,
            [this](GtProject* proj) {
        assert(proj == helper::findProject(uuid()));
        if (isOpen())
        {
            populateFileObjects();
            initFileWatcher();
        }
    });

    connect(&m_pimpl->deleteMissingFiles, &GtBoolProperty::changed,
            this, [this](){ updateRefreshRequired(); });

    connect(&m_pimpl->deleteFilteredOutFiles, &GtBoolProperty::changed,
            this, [this](){ updateRefreshRequired(); });

    connect(&m_pimpl->fileFilters, &GtStringProperty::changed,
            this, [this](){ updateRefreshRequired(); });
}

Directory::Directory(const QString& localPath) :
    Directory(gt::resource::url::fromAbsPath(localPath)) { }

Directory::~Directory() = default;

bool
Directory::open()
{
    if (!exists()) return false;

    auto _ = gtApp->makeCommand(
        this, QString{"%1 %2"}.arg(tr("Open"), objectName()));

    m_pimpl->isOpen = true;

    initFileWatcher();
    populateFileObjects();

    return true;
}

void
Directory::close()
{
    if (!m_pimpl->isOpen) return;

    auto _ = gtApp->makeCommand(
        this, QString{"%1 %2"}.arg(tr("Close"), objectName()));

    m_pimpl->isOpen = false;
    m_pimpl->isRefreshRequired = false;

    m_pimpl->watcher = nullptr;

    gtDataModel->deleteFromModel(findDirectChildren());
}

void
Directory::refresh()
{
    auto _ = gtApp->makeCommand(
        this, QString{"%1 %2"}.arg(tr("Refresh"), objectName()));

    populateFileObjects();
}

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
Directory::isRefreshRequired() const
{
    return m_pimpl->isRefreshRequired;
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
Directory::setIsOpen(bool isOpen)
{
    m_pimpl->isOpen = isOpen;
}

void
Directory::hideFileFiltersProperty(bool hide)
{
    m_pimpl->fileFilters.hide(hide);
    m_pimpl->fileFilters.setReadOnly(hide);
}

void
Directory::hideDeleteFilteredOutProperty(bool hide)
{
    m_pimpl->deleteFilteredOutFiles.hide(hide);
    m_pimpl->deleteFilteredOutFiles.setReadOnly(hide);
}

void
Directory::hideDeleteMissingProperty(bool hide)
{
    m_pimpl->deleteMissingFiles.hide(hide);
    m_pimpl->deleteMissingFiles.setReadOnly(hide);
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

QList<File*>
Directory::fileList()
{
    return findDirectChildren<File*>();
}

QList<const File*>
Directory::fileList() const
{
    return findDirectChildren<const File*>();
}

QSet<QString>
Directory::filteredDiskPaths() const
{
    QDir dir{path()};
    auto filters = fileFilters();

    dir.setNameFilters(filters.isEmpty() ? QStringList{"*"} : filters);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name | QDir::IgnoreCase);

    QSet<QString> paths{};

    const auto& fileInfos = dir.entryInfoList();
    for (const auto& fi : fileInfos)
    {
        paths.insert(QDir::cleanPath(fi.absoluteFilePath()));
    }

    return paths;
}

QSet<QString>
Directory::pathsInModel() const
{
    QSet<QString> paths{};

    const auto& files = fileList();
    for (auto* f : files) paths.insert(f->path());

    return paths;
}

QSet<QString>
Directory::pathsNotInModel() const
{
    return filteredDiskPaths() - pathsInModel();
}

void
Directory::initFileWatcher()
{
    // set up a file system watcher and single-shot timer to debounce rapid
    // directory changes. This ensuring refresh() is called once even if
    // multiple files are deleted at once.
    m_pimpl->watcher = std::make_unique<QFileSystemWatcher>();
    m_pimpl->watcherTimer = std::make_unique<QTimer>();

    m_pimpl->watcherTimer->setSingleShot(true);
    m_pimpl->watcherTimer->setInterval(100);

    connect(m_pimpl->watcherTimer.get(), &QTimer::timeout, this, [this]() {
        if (exists() && isOpen()) refresh(); });
    connect(m_pimpl->watcher.get(), &QFileSystemWatcher::directoryChanged,
            this, [this](const QString&) { m_pimpl->watcherTimer->start(); });

    m_pimpl->watcher->addPath(path());
}

void
Directory::populateFileObjects()
{
    // Note: this method directly interacts with the core data model to bypass
    // undo/redo command creation

    auto _refresh = gt::finally([this](){
        m_pimpl->isRefreshRequired = false; });

    m_pimpl->lastFileFilters = m_pimpl->fileFilters;

    QList<GtObject*> objList{};
    QSet<QString> existingPaths{};

    auto fileObjs = fileList();

    // if enabled, delete file objects whose underlying file is missing
    if (m_pimpl->deleteMissingFiles)
    {
        QList<GtObject*> toDelete{};

        for (auto* f : qAsConst(fileObjs)) if (!f->exists()) toDelete.append(f);

        if (!toDelete.isEmpty())
        {
            gtDataModel->reduceToParents(toDelete);
            gtDataModel->GtCoreDatamodel::deleteFromModel(toDelete);
        }

        fileObjs = fileList();
    }

    // apply filter to existing file objects
    auto filters = fileFilters();

    if (m_pimpl->deleteFilteredOutFiles)
    {
        QList<GtObject*> toDelete{};

        for (auto* f : qAsConst(fileObjs))
        {
            if (!fileMatchesFilter(f->path(), filters)) toDelete.append(f);
        }

        if (!toDelete.isEmpty())
        {
            gtDataModel->reduceToParents(toDelete);
            gtDataModel->GtCoreDatamodel::deleteFromModel(toDelete);
        }

        fileObjs = fileList();
    }
    else
    {
        for (auto* f : qAsConst(fileObjs))
        {
            f->setUserHidden(!fileMatchesFilter(f->path(), filters));
        }
    }

    // collect existing file objects
    for (auto* f : qAsConst(fileObjs))
    {
        existingPaths.insert(f->path());
        objList.append(f);
    }

    // create file objects for disk files that do not already exist in the model
    const QSet<QString>& pathsOnDisk = filteredDiskPaths();
    for (const auto& path : pathsOnDisk)
    {
        if (existingPaths.contains(path)) continue;

        auto fileObj = createFileResource(QFileInfo{path});
        objList.append(fileObj.release());
    }

    // if no new abjects were created, return
    if (objList.isEmpty() || fileObjs.size() == objList.size()) return;

    // sort file objects using natural sort order based on their object names
    QCollator collator;
    collator.setNumericMode(true);
    collator.setCaseSensitivity(Qt::CaseInsensitive);

    std::sort(objList.begin(), objList.end(),
              [&](GtObject* a, GtObject* b) {
        return collator.compare(a->objectName(), b->objectName()) < 0;
    });

    // append the file objects in the sorted order
    gtDataModel->beginResetModelView();
    auto _end = gt::finally([](){ gtDataModel->endResetModelView(); });

    for (auto* obj : objList) obj->disconnectFromParent();

    gtDataModel->GtCoreDatamodel::appendChildren(objList, this);
}

void
Directory::updateRefreshRequired()
{
    if (!exists() || !isOpen())
    {
        m_pimpl->isRefreshRequired = false;
        return;
    }

    bool requiresRefresh = false;

    // check if file filters changed
    requiresRefresh |= (m_pimpl->lastFileFilters != m_pimpl->fileFilters.get());

    // check if missing files need to be deleted
    if (m_pimpl->deleteMissingFiles)
    {
        auto files = fileList();
        requiresRefresh |= std::any_of(
            files.begin(), files.end(), [](auto* f){ return !f->exists(); });
    }

    // check if filtered-out files need to be deleted
    if (m_pimpl->deleteFilteredOutFiles)
    {
        auto files = fileList();
        requiresRefresh |= std::any_of(
            files.begin(), files.end(),
            [](auto* f){ return f->isUserHidden(); });
    }

    m_pimpl->isRefreshRequired = requiresRefresh;
}

} // namespace data

} // namespace resource

} // namespace gt
