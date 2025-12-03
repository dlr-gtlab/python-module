/* GTlab - Gas Turbine laboratory
 * Source File: file.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef FILE_H
#define FILE_H

#include <QUrl>
#include <QFileInfo>

#include "gt/resource/data/streamresource.h"


namespace gt
{

namespace resource
{

namespace data
{

class File : public StreamResource
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit File(const QUrl& url = {});
    explicit File(const QString& localPath);

    ~File() override;

    Q_INVOKABLE bool exists() const override;

    Q_INVOKABLE QString path() const;

    QFileInfo info() const;

    std::unique_ptr<QIODevice> open(QIODevice::OpenMode mode) const override;

protected:
    virtual QString toPath(const QUrl& url) const;
};

} // namespace data

} // namespace resource

} // namespace gt

#endif // FILE_H
