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

GtpyDirectoryUI::GtpyDirectoryUI()
{

    auto dirExists = [](GtObject* obj) {
        auto* dir = qobject_cast<gt::resource::data::Directory*>(obj);
        return dir ? dir->exists() : false;
    };

    addSingleAction(tr("Show in Explorer"),  [](GtObject* obj) {
        auto* dir = qobject_cast<gt::resource::data::Directory*>(obj);
        if (!dir) return;

        QDesktopServices::openUrl(
            QUrl::fromLocalFile(QDir{dir->path()}.absolutePath()));
    })
        .setIcon(gt::gui::icon::folderOpen())
        .setVerificationMethod(dirExists);
}

QIcon
GtpyDirectoryUI::icon(GtObject* obj) const
{
    return gt::gui::icon::python();
}
