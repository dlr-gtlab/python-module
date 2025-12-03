/* GTlab - Gas Turbine laboratory
 * Source File: projectfile.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt/resource/data/projectfile.h"

#include <gt_datamodel.h>

#include "gt/resource/url.h"
#include "gt/resource/data/helper.h"

namespace
{

QString makeUniqueFileName(const QString& newName, const QFileInfo& fi)
{
    QString baseName = newName;
    QDir dir = fi.absoluteDir();
    const QString suffix = fi.suffix();
    const QStringList fileList = dir.entryList(QDir::Files);

    if (baseName.endsWith("." + suffix, Qt::CaseInsensitive))
    {
        baseName.chop(suffix.length() + 1);
    }

    QSet<QString> existing;
    for (const auto& f :fileList ) existing.insert(f.toLower());

    QString candidate = baseName + "." + suffix;
    if (!existing.contains(candidate.toLower())) return candidate;

    int counter = 1;

    while (true)
    {
        candidate = QString{"%1[%2]"}.arg(baseName).arg(counter);
        candidate += "." + suffix;

        if (!existing.contains(candidate.toLower())) break;

        counter++;
    }

    return candidate;
}

}

namespace gt
{

namespace resource
{

namespace data
{

struct ProjectFile::Impl
{
    Impl(File* file)
    {
        fileContent = new helper::FileContent{file};

        // create an externalizable file content helper
        fileContent->setObjectName("fileContent");
        fileContent->setDefault(true);
        fileContent->setUserHidden(true);

        // set the 'isFetched' property of the file content helper to false
        auto* isFetchedProp = fileContent->findProperty("isFetched");
        assert(isFetchedProp);
        if (isFetchedProp) isFetchedProp->setValueFromVariant(false);
    }

    QPointer<helper::FileContent> fileContent;
};

ProjectFile::ProjectFile(const QUrl& url) :
    File(url),
    m_pimpl(std::make_unique<Impl>(this))
{
    setFlag(GtObject::UserRenamable, true);

    // append the helper as a child
    appendChild(m_pimpl->fileContent);

    // set up connections
    connect(m_pimpl->fileContent, &helper::FileContent::contentChanged, this,
            &ProjectFile::changed);
    connect(this, &ProjectFile::objectNameChanged, this,
            [this](const QString& name) {

        if (!isRenamable()) return;

        QSignalBlocker blocker{this};
        renameFile(name);
    });
}

ProjectFile::ProjectFile(const QString& relPath) :
    ProjectFile(gt::resource::url::fromProjRelPath(relPath)) { }

ProjectFile::~ProjectFile() = default;

QByteArray
ProjectFile::readAll()
{
    return fileContent().get();
}

bool
ProjectFile::writeAll(const QByteArray& content)
{
    fileContent().set(content);
    return true;
}

QString
ProjectFile::toPath(const QUrl& url) const
{
    // TODO: The 'this' pointer cannot be used to find the parent project,
    // because when accessing the URL while executing a task,
    // the instance has no parent of type GtProject.
    // Consider storing the project as a member variable of type QPointer.

    auto* proj = helper::findProject(uuid());
    if (!proj)
    {
        // gtError() << tr("Unable to resolve the parent project for %1. "
        //                 "The absolute path of the file cannot be determined.")
        //                  .arg(objectName()) << " (" << this << ")";
        return {};
    }

    return gt::resource::url::toAbsPath(proj, url);
}

helper::FileContent&
ProjectFile::fileContent() const
{
    assert(m_pimpl->fileContent);
    return *m_pimpl->fileContent;
}

void
ProjectFile::renameFile(const QString& newName)
{
    const QFileInfo fi = info();
    if (!fi.exists() || !fi.isFile()) return;

    QString uniqueName = makeUniqueFileName(newName, fi);

    gtDebug() << uniqueName;
    QString newFilePath = fi.absolutePath() + QDir::separator() + uniqueName;

    QFile file{fi.absoluteFilePath()};

    QUrl oldUrl = url();
    QUrl newUrl = gt::resource::url::toProjFileUrl(
        helper::findProject(uuid()), newFilePath);

    setUrl(newUrl);

    if (file.rename(newFilePath))
    {
        setObjectName(uniqueName);
    }
    else
    {
        setObjectName(fi.fileName());
        setUrl(oldUrl);
    }
}

} // namespace data

} // namespace resource

} // namespace gt
