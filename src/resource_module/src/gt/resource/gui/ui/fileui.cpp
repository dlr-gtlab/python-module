/* GTlab - Gas Turbine laboratory
 * Source File: ResourcesModule
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 04.11.2025
 * Author: Marvin Nöthen (DLR AT-TWK)
 */

#include "gt/resource/gui/ui/fileui.h"

#include <QDir>
#include <QFileInfo>
#include <QDesktopServices>

#include <gt_icons.h>
#include <gt_colors.h>

#include "gt/resource/data/file.h"
#include "gt/resource/gui/mdi/fileviewer.h"

namespace gt
{

namespace resource
{

namespace gui
{

namespace ui
{

FileUI::FileUI()
{
    using File = gt::resource::data::File;

    auto open = [](GtObject* obj, bool location) {
        auto* res = qobject_cast<File*>(obj);
        if (!res) return;

        QFileInfo info{res->url().toLocalFile()};
        QDesktopServices::openUrl(
            location ? QUrl::fromLocalFile(info.dir().absolutePath()) :
                QUrl::fromLocalFile(info.absoluteFilePath()));
    };

    auto fileExists = [](GtObject* obj) {
        auto* res = qobject_cast<File*>(obj);
        return res ? QFileInfo::exists(res->url().toLocalFile()) : false;
    };

    auto dirExists = [](GtObject* obj) {
        auto* res = qobject_cast<File*>(obj);
        return res ? QFileInfo{res->url().toLocalFile()}.dir().exists() : false;
    };

    addSingleAction(tr("Open in Default Application"),
                    [open](GtObject* obj) { open(obj, false); })
        .setIcon(gt::gui::icon::file())
        .setVerificationMethod(fileExists);

    addSingleAction(tr("Open file location"),
                    [open](GtObject* obj) { open(obj, true); })
        .setIcon(gt::gui::icon::folderOpen())
        .setVerificationMethod(dirExists);
}

QIcon
FileUI::icon(GtObject* obj) const
{
    auto color = gt::gui::color::text();

    if (auto* res = qobject_cast<gt::resource::data::File*>(obj))
    {
        color = res->exists() ? color : gt::gui::color::disabled();
    }

    return gt::gui::colorize(gt::gui::icon::file(), color);
}

QStringList
FileUI::openWith(GtObject *obj)
{
    using File = gt::resource::data::File;
    using FileViewer = gt::resource::gui::mdi::FileViewer;

    QStringList list;

    if (auto* res = qobject_cast<File*>(obj))
    {
        if (res->exists()) list << GT_CLASSNAME(FileViewer);
    }

    return list;
}

QRegExp
FileUI::validatorRegExp()
{
    return QRegExp{".*"};
}

QVariant
FileUI::specificData(GtObject* obj, int role, int column) const
{
    auto* res = qobject_cast<gt::resource::data::File*>(obj);
    if (!res) return {};

    if (column == 0)
    {
        switch (role)
        {

        case Qt::ToolTipRole:
        {
            if (!res->exists()) return tr("File does not exist!");

            break;
        }

        case Qt::ForegroundRole:
        {
            if (!res->exists()) return gt::gui::color::disabled();

            break;
        }

        }
    }

    return {};
}

} // namespace ui

} // namespace gui

} // namespace resource

} // namespace gt

