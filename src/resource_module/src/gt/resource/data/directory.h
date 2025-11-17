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

    bool exists() const;

    bool open();

    void close();

    bool isOpen() const;

    const QStringList& fileFilters() const;

    void setFileFilters(const QStringList& filters);

protected:
    virtual void refresh();

    virtual std::unique_ptr<File> createFileResource(const QFileInfo& file);

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    void clearChildren();
};

} // namespace data

} // namespace resource

} // namespace gt


#endif // DIRECTORY_H
