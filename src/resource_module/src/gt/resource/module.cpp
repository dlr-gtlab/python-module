/* GTlab - Gas Turbine laboratory
 * Source File: ResourcesModule
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 04.11.2025
 * Author: Marvin Nöthen (DLR AT-TWK)
 */

#include "gt/resource/module.h"

#include "gt/resource/data/file.h"
#include "gt/resource/data/package.h"
#include "gt/resource/data/scripts.h"
#include "gt/resource/data/directory.h"
#include "gt/resource/data/projectfile.h"
#include "gt/resource/data/httpresource.h"
#include "gt/resource/data/projectdirectory.h"
#include "gt/resource/data/helper/filecontent.h"

#include "gt/resource/gui/ui/fileui.h"
#include "gt/resource/gui/ui/scriptsui.h"
#include "gt/resource/gui/ui/packageui.h"
#include "gt/resource/gui/ui/directoryui.h"
#include "gt/resource/gui/ui/httpresourceui.h"

#include "gt/resource/gui/mdi/fileviewer.h"

namespace gt
{

namespace resource
{

// GtVersionNumber
// Module::version()
// {
//     return GtVersionNumber{0, 0, 1, "alpha"};
// }

// QString
// Module::description() const
// {
//     return QStringLiteral("GTlab Resources Module");
// }

// void
// Module::init() { }

// Module::MetaInformation
// Module::metaInformation() const
// {
//     MetaInformation m;

//     m.author =        QStringLiteral("Marvin Nöthen (DLR AT-TWK)");
//     m.authorContact = QStringLiteral("marvin.noethen@dlr.de");

//     return m;
// }

QMetaObject
Module::package()
{
    using namespace data;

    return GT_METADATA(Package);
}

QList<QMetaObject>
Module::data()
{
    using namespace data;

    QList<QMetaObject> list;

    list << GT_METADATA(Scripts);
    list << GT_METADATA(File);
    list << GT_METADATA(ProjectFile);
    list << GT_METADATA(helper::FileContent);
    list << GT_METADATA(Directory);
    list << GT_METADATA(ProjectDirectory);
    list << GT_METADATA(HttpResource);

    return list;
}

bool
Module::standAlone()
{
    return true;
}

QList<QMetaObject>
Module::mdiItems()
{
    using namespace gui::mdi;

    QList<QMetaObject> list;

    list << GT_METADATA(FileViewer);

    return list;
}

QList<QMetaObject>
Module::dockWidgets()
{   
    return {};
}

QMap<const char*, QMetaObject>
Module::uiItems()
{
    using namespace data;
    using namespace gui::ui;

    QMap<const char*, QMetaObject> map;

    map.insert(GT_CLASSNAME(Package), GT_METADATA(PackageUI));
    map.insert(GT_CLASSNAME(Scripts), GT_METADATA(ScriptsUI));
    map.insert(GT_CLASSNAME(File), GT_METADATA(FileUI));
    map.insert(GT_CLASSNAME(ProjectFile), GT_METADATA(FileUI));
    map.insert(GT_CLASSNAME(Directory),GT_METADATA(DirectoryUI));
    map.insert(GT_CLASSNAME(ProjectDirectory),GT_METADATA(DirectoryUI));
    map.insert(GT_CLASSNAME(HttpResource),GT_METADATA(HttpResourceUI));

    return map;
}

QList<QMetaObject>
Module::postItems()
{
    QList<QMetaObject> list;

    return list;
}

QList<QMetaObject>
Module::postPlots()
{
    QList<QMetaObject> list;

    return list;
}

QList<QMetaObject>
Module::importer() const
{
    QList<QMetaObject> list;

    return list;
}

} // namespace resource

} // namespace gt

