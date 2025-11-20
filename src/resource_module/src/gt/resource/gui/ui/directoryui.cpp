/* GTlab - Gas Turbine laboratory
 * Source File: ResourcesModule
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 04.11.2025
 * Author: Marvin Nöthen (DLR AT-TWK)
 */

#include "gt/resource/gui/ui/directoryui.h"

#include <QDir>
#include <QDesktopServices>

#include <gt_icons.h>
#include <gt_colors.h>

#include "gt/resource/data/directory.h"

namespace gt
{

namespace resource
{

namespace gui
{

namespace ui
{

DirectoryUI::DirectoryUI()
{
    using Directory = gt::resource::data::Directory;

    auto isOpen = [](GtObject* obj) {
        auto* dir = qobject_cast<Directory*>(obj);
        return dir ? dir->isOpen() : false;
    };

    auto isNotOpenButExists = [](GtObject* obj) {
        auto* dir = qobject_cast<Directory*>(obj);
        return dir ? dir->exists() && !dir->isOpen() : false;
    };

    auto isOpenAndExists = [](GtObject* obj) {
        auto* dir = qobject_cast<Directory*>(obj);
        return dir ? dir->exists() && dir->isOpen() : false;
    };

    addSingleAction(tr("Open Directory"), [](GtObject* obj) {
        auto* dir = qobject_cast<Directory*>(obj);
        if (dir) dir->open();
    })
        .setIcon(gt::gui::icon::folderOpen())
        .setVerificationMethod(isNotOpenButExists);

    addSingleAction(tr("Close Directory"), [](GtObject* obj) {
        auto* dir = qobject_cast<Directory*>(obj);
        if (dir) dir->close();
    })
        .setIcon(gt::gui::icon::folder())
        .setVerificationMethod(isOpen);

    addSeparator();

    addSingleAction(tr("Clean"), [](GtObject* obj) {
        auto* dir = qobject_cast<Directory*>(obj);
        if (dir) dir->clean();
    })
        .setIcon(gt::gui::icon::clear())
        .setVerificationMethod(isOpenAndExists);

    addSeparator();

    auto dirExists = [](GtObject* obj) {
        auto* dir = qobject_cast<Directory*>(obj);
        return dir ? dir->exists() : false;
    };

    addSingleAction(tr("Show in Explorer"),  [](GtObject* obj) {
        auto* dir = qobject_cast<Directory*>(obj);
        if (!dir) return;

        QDesktopServices::openUrl(
            QUrl::fromLocalFile(QDir{dir->path()}.absolutePath()));
    })
        .setIcon(gt::gui::icon::folderOpen())
        .setVerificationMethod(dirExists);
}

QIcon
DirectoryUI::icon(GtObject* obj) const
{
    auto* res = qobject_cast<gt::resource::data::Directory*>(obj);
    if (!res) return gt::gui::icon::folder();

    auto color = res->exists() ? gt::gui::color::text() :
                     gt::gui::color::disabled();
    auto icon = res->isOpen() ? gt::gui::icon::folderOpen() :
                    gt::gui::icon::folder();

    return gt::gui::colorize(icon, color);
}

QRegExp
DirectoryUI::validatorRegExp()
{
    return QRegExp{".*"};
}

QVariant
DirectoryUI::specificData(GtObject* obj, int role, int column) const
{
    auto* res = qobject_cast<gt::resource::data::Directory*>(obj);
    if (!res) return {};

    if (column == 0)
    {
        switch (role)
        {

        case Qt::ToolTipRole:
        {
            if (!res->exists()) return tr("Directory does not exist!");

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
