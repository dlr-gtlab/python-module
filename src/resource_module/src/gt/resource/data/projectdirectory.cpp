/* GTlab - Gas Turbine laboratory
 * Source File: projectdirectory.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 17.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt/resource/data/projectdirectory.h"

#include <gt_datamodel.h>

#include "gt/resource/url.h"
#include "gt/resource/data/helper.h"
#include "gt/resource/data/projectfile.h"

namespace gt
{

namespace resource
{

namespace data
{

ProjectDirectory::ProjectDirectory(const QUrl& url) : Directory(url) { }

ProjectDirectory::ProjectDirectory(const QString& relPath) :
    ProjectDirectory(gt::resource::url::fromProjRelPath(relPath)) { }

ProjectDirectory::~ProjectDirectory() = default;

QString
ProjectDirectory::toPath(const QUrl& url) const
{
    // TODO: The 'this' pointer cannot be used to find the parent project,
    // because when accessing the URL while executing a task,
    // the instance has no parent of type GtProject.
    // Consider storing the project as a member variable of type QPointer.

    auto* proj = helper::findProject(uuid());
    if (!proj)
    {
        // gtError() << tr("Unable to resolve the parent project for %1. "
        //                 "The absolute path of the directory cannot be "
        //                 "determined.").arg(objectName()) << " (" << this << ")";
        return {};
    }

    return gt::resource::url::toAbsPath(helper::findProject(uuid()), url);
}

std::unique_ptr<File>
ProjectDirectory::createFileResource(const QFileInfo& file)
{
    return std::make_unique<ProjectFile>(gt::resource::url::toProjFileUrl(
            helper::findProject(uuid()), file.absoluteFilePath()));
}

} // namespace data

} // namespace resource

} // namespace gt
