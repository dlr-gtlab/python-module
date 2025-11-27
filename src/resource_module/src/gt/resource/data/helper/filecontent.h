/* GTlab - Gas Turbine laboratory
 * Source File: filecontent.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef FILECONTENT_H
#define FILECONTENT_H

#include <gt_externalizedobject.h>

namespace gt
{

namespace resource
{

namespace data
{

class File;

namespace helper
{

class FileContent;
class FileContentData : public GtExternalizedObjectData
{
    GT_DECL_BASECLASS(FileContent)

public:
    FileContentData(FileContent* base);

    const QByteArray& content() const;

    void setContent(const QByteArray& content);
};

class FileContent : public GtExternalizedObject
{
    Q_OBJECT

    Q_PROPERTY(QByteArray content READ contentValue WRITE set NOTIFY contentChanged)

    GT_DECL_DATACLASS(FileContentData)

public:
    Q_INVOKABLE explicit FileContent(
        ::gt::resource::data::File* file = nullptr);

    const QByteArray& get();

    void set(const QByteArray& content);

protected:
    bool isDataValid() const override;

    bool canExternalize() const override;

    bool doFetchData(QVariant& metaData, bool fetchInitialVersion) override;

    bool doExternalizeData(QVariant& metaData) override;

    void doClearExternalizedData() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    ::gt::resource::data::File* filePtr() const;

    const QByteArray& contentValue();

signals:
    void contentChanged();
};

} // namespace helper

} // namespace data

} // namespace resource

} // namespace gt

#endif // FILECONTENT_H
