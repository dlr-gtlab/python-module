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
#include <QDesktopServices>

#include <gt_icons.h>
#include <gt_colors.h>

#include "gt/resource/data/file.h"
#include "gt/resource/gui/icons.h"
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
        auto* file = qobject_cast<File*>(obj);
        if (!file) return;

        QFileInfo info = file->info();
        QDesktopServices::openUrl(
            location ? QUrl::fromLocalFile(info.dir().absolutePath()) :
                QUrl::fromLocalFile(info.absoluteFilePath()));
    };

    auto fileExists = [](GtObject* obj) {
        auto* file = qobject_cast<File*>(obj);
        return file ? QFileInfo::exists(file->path()) : false;
    };

    auto dirExists = [](GtObject* obj) {
        auto* file = qobject_cast<File*>(obj);
        return file ? file->info().dir().exists() : false;
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
    auto* file = qobject_cast<gt::resource::data::File*>(obj);
    if (!file) return {};

    if (!file->exists())
    {
        return gt::gui::colorize(gt::resource::gui::icon::fileMissing(),
                                 gt::gui::color::disabled());
    }

    return gt::gui::icon::file();
}

QStringList
FileUI::openWith(GtObject* obj)
{
    using File = gt::resource::data::File;
    using FileViewer = gt::resource::gui::mdi::FileViewer;

    QStringList list;

    if (auto* file = qobject_cast<File*>(obj))
    {
        if (file->exists()) list << GT_CLASSNAME(FileViewer);
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
    auto* file = qobject_cast<gt::resource::data::File*>(obj);
    if (!file) return {};

    switch (role)
    {

    case Qt::ToolTipRole:
    {
        if (!file->exists()) return tr("File does not exist!");
        break;
    }

    case Qt::ForegroundRole:
    {
        if (!file->exists()) return gt::gui::color::disabled();
        break;
    }

    case Qt::DecorationRole:
    {
        if (column == 1 && !file->exists())
        {
            return gt::gui::colorize(gt::gui::icon::exclamationmark(),
                                     gt::gui::color::disabled());
        }

        break;
    }

    default:
        break;

    }

    return {};
}

} // namespace ui

} // namespace gui

} // namespace resource

} // namespace gt

