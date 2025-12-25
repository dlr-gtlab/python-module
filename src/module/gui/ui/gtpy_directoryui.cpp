/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_directoryui.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 18.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_directoryui.h"

#include <QDir>
#include <QDesktopServices>

#include <gt/resource/data/directory.h>

#include <gt_icons.h>
#include <gt_colors.h>

GtpyDirectoryUI::GtpyDirectoryUI()
{

    auto dirExists = [](GtObject* obj) {
        auto* dir = qobject_cast<gt::resource::data::Directory*>(obj);
        return dir ? dir->exists() : false;
    };

    auto isRefreshRequired = [](GtObject* obj) {
        auto* dir = qobject_cast<gt::resource::data::Directory*>(obj);
        return dir ? dir->exists() && dir->isOpen() && dir->isRefreshRequired()
                   : false;
    };

    addSingleAction(tr("Show in Explorer"),  [](GtObject* obj) {
        auto* dir = qobject_cast<gt::resource::data::Directory*>(obj);
        if (!dir) return;

        QDesktopServices::openUrl(
            QUrl::fromLocalFile(QDir{dir->path()}.absolutePath()));
    })
        .setIcon(gt::gui::icon::folderOpen())
        .setVerificationMethod(dirExists);

    addSeparator();

    addSingleAction(tr("Refresh"), [isRefreshRequired](GtObject* obj) {
        auto* dir = qobject_cast<gt::resource::data::Directory*>(obj);
        if (dir && isRefreshRequired(dir)) dir->refresh();
    })
        .setIcon(gt::gui::icon::reload())
        .setVerificationMethod(isRefreshRequired)
        .setShortCut(QKeySequence{Qt::Key_F5});
}

QIcon
GtpyDirectoryUI::icon(GtObject* obj) const
{
    return gt::gui::icon::python();
}

QVariant
GtpyDirectoryUI::specificData(GtObject *obj, int role, int col) const
{
    auto* dir = qobject_cast<gt::resource::data::Directory*>(obj);
    if (!dir) return {};

    switch (role)
    {

    case Qt::DecorationRole:
    {
        if (col != 1 || !dir->isRefreshRequired()) break;

        auto c = dir->hasChanges() ? gt::gui::color::changedObjectForeground()
                                   : gt::gui::color::text();
        return gt::gui::colorize(gt::gui::icon::reload(), c);
    }

    default:
        break;

    }

    return {};
}
