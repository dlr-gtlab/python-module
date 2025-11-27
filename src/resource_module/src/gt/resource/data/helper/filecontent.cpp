/* GTlab - Gas Turbine laboratory
 * Source File: filecontent.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt/resource/data/helper/filecontent.h"

#include <gt_datamodel.h>
#include <gt_objectlinkproperty.h>

#include "gt/resource/data/file.h"

namespace gt
{

namespace resource
{

namespace data
{

namespace helper
{

using File = gt::resource::data::File;

struct FileContent::Impl
{
    Impl(File* f, GtObject* parent) :
        file{f},
        fileLink{"fileLink", QObject::tr("fileLink"), QObject::tr("fileLink"),
                   parent, QStringList{GT_CLASSNAME(File)}}
    {
        fileLink.setReadOnly(true);
        if (file) fileLink.setVal(file->uuid());
    }

    QPointer<File> file;
    QByteArray content{};
    GtObjectLinkProperty fileLink;
};

FileContentData::FileContentData(FileContent* base) :
    GtExternalizedObjectData{base} { }

const QByteArray&
FileContentData::content() const
{
    assert(m_base);
    return base()->m_pimpl->content;
}

void
FileContentData::setContent(const QByteArray& content)
{
    base()->m_pimpl->content = content;
}

FileContent::FileContent(File* file) :
    m_pimpl(std::make_unique<Impl>(file, this))
{
    registerProperty(m_pimpl->fileLink);
}

const QByteArray&
FileContent::get()
{
    internalize();
    return fetchData().content();
}

void
FileContent::set(const QByteArray& content)
{
    internalize();
    auto data = fetchData();
    data.setContent(content);

    emit contentChanged();
}

const QByteArray&
FileContent::contentValue()
{
    return m_pimpl->content;
}

bool
FileContent::isDataValid() const
{
    return filePtr() && GtExternalizedObject::isDataValid();
}

bool
FileContent::canExternalize() const
{
    return isDataValid();
}

bool
FileContent::doFetchData(QVariant&, bool)
{
    auto* file = filePtr();
    if (!file || !file->exists()) return false;

    auto device = file->open(QIODevice::ReadOnly);
    if (!device) return false;

    m_pimpl->content = device->readAll();

    return true;
}

bool
FileContent::doExternalizeData(QVariant&)
{
    auto* file = filePtr();
    if (!file || !file->exists()) return false;

    auto device = file->open(QIODevice::WriteOnly);
    if (!device) return false;

    qint64 written = device->write(m_pimpl->content);
    return written == m_pimpl->content.size();
}

void
FileContent::doClearExternalizedData()
{
    m_pimpl->content.clear();
}

File*
FileContent::filePtr() const
{
    if (!m_pimpl->file)
    {
        m_pimpl->file = qobject_cast<File*>(
            gtDataModel->objectByUuid(m_pimpl->fileLink.linkedObjectUUID()));
    }

    assert(m_pimpl->file);
    return m_pimpl->file;
}

} // namespace helper

} // namespace data

} // namespace resource

} // namespace gt
