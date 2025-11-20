/* GTlab - Gas Turbine laboratory
 * Source File: projectdirectory.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef PROJECTDIRECTORY_H
#define PROJECTDIRECTORY_H

#include "gt/resource/data/directory.h"

namespace gt
{

namespace resource
{

namespace data
{

class ProjectDirectory : public Directory
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit ProjectDirectory(const QUrl& url = {});
    explicit ProjectDirectory(const QString& relPath);

    ~ProjectDirectory() override;

protected:
    QString toPath(const QUrl& url) const override;

    std::unique_ptr<File> createFileResource(const QFileInfo& file) override;
};

} // namespace data

} // namespace resource

} // namespace gt

#endif // PROJECTDIRECTORY_H
