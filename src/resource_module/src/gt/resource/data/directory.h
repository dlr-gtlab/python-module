/* GTlab - Gas Turbine laboratory
 * Source File: directory.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "gt/resource/data/urlresource.h"

class QFileInfo;

namespace gt
{

namespace resource
{

namespace data
{

class File;

class Directory : public UrlResource
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit Directory(const QUrl& url = {});
    explicit Directory(const QString& localPath);

    ~Directory() override;

    bool open();

    void close();

    void refresh();

    bool exists() const;

    bool isOpen() const;

    bool isRefreshRequired() const;

    QFileInfo info() const;

    Q_INVOKABLE QString path() const;

    Q_INVOKABLE QStringList fileFilters() const;

    void setFileFilters(const QStringList& filters);

protected:
    void setIsOpen(bool isOpen = true);

    void hideFileFiltersProperty(bool hide = true);

    void hideDeleteFilteredOutProperty(bool hide = true);

    void hideDeleteMissingProperty(bool hide = true);

    virtual std::unique_ptr<File> createFileResource(const QFileInfo& file);

    virtual QString toPath(const QUrl& url) const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    QList<File*> fileList();
    QList<const File*> fileList() const;

    QSet<QString> filteredDiskPaths() const;

    QSet<QString> pathsInModel() const;

    QSet<QString> pathsNotInModel() const;

    void initFileWatcher();

    void populateFileObjects();

    void updateRefreshRequired();
};

} // namespace data

} // namespace resource

} // namespace gt


#endif // DIRECTORY_H
