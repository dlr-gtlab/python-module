/* GTlab - Gas Turbine laboratory
 * Source File: projectfile.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef PROJECTFILE_H
#define PROJECTFILE_H


#include "gt/resource/data/file.h"
#include "gt/resource/data/helper/filecontent.h"

namespace gt
{

namespace resource
{

namespace data
{

class ProjectFile : public File
{
    Q_OBJECT

    friend class helper::FileContent;

public:
    Q_INVOKABLE explicit ProjectFile(const QUrl& url = {});
    explicit ProjectFile(const QString& relPath);

    ~ProjectFile() override;

    QByteArray readAll() override;

    bool writeAll(const QByteArray& content) override;

protected:
    QString toPath(const QUrl& url) const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    helper::FileContent& fileContent() const;

private slots:
    void renameFile(const QString& newName);
};

} // namespace data

} // namespace resource

} // namespace gt

#endif // PROJECTFILE_H
