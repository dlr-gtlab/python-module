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

    auto isRefreshRequired = [](GtObject* obj) {
        auto* dir = qobject_cast<Directory*>(obj);
        return dir ? dir->exists() && dir->isOpen() && dir->isRefreshRequired()
                   : false;
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

    addSingleAction(tr("Refresh"), [isRefreshRequired](GtObject* obj) {
        auto* dir = qobject_cast<Directory*>(obj);
        if (dir && isRefreshRequired(dir)) dir->refresh();
    })
        .setIcon(gt::gui::icon::reload())
        .setVerificationMethod(isRefreshRequired)
        .setShortCut(QKeySequence{Qt::Key_F5});

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
    auto* dir = qobject_cast<gt::resource::data::Directory*>(obj);
    if (!dir) return gt::gui::icon::folder();

    auto color = dir->exists() ? gt::gui::color::text() :
                     gt::gui::color::disabled();
    auto icon = dir->isOpen() ? gt::gui::icon::folderOpen() :
                    gt::gui::icon::folder();

    return gt::gui::colorize(icon, color);
}

QRegExp
DirectoryUI::validatorRegExp()
{
    return QRegExp{".*"};
}

void
DirectoryUI::doubleClicked(GtObject* obj)
{
    auto* dir = qobject_cast<gt::resource::data::Directory*>(obj);
    if (!dir) return;

    if (dir->isOpen()) dir->close();
    else dir->open();
}

QVariant
DirectoryUI::specificData(GtObject* obj, int role, int col) const
{
    auto* dir = qobject_cast<gt::resource::data::Directory*>(obj);
    if (!dir) return {};

    switch (role)
    {

    case Qt::ToolTipRole:
    {
        if (!dir->exists()) return tr("Directory does not exist!");
        else if (col == 1 && dir->isRefreshRequired())
        {
            return tr("%1 needs to be refreshed.").arg(dir->objectName());
        }

        break;
    }

    case Qt::ForegroundRole:
    {
        if (!dir->exists()) return gt::gui::color::disabled();
        break;
    }

    case Qt::DecorationRole:
    {
        if (col != 1) break;

        if (!dir->exists())
        {
            return gt::gui::colorize(gt::gui::icon::exclamationmark(),
                                     gt::gui::color::disabled());
        }
        else if (dir->isRefreshRequired())
        {
            auto c = dir->hasChanges()
                         ? gt::gui::color::changedObjectForeground()
                         : gt::gui::color::text();

            return gt::gui::colorize(gt::gui::icon::reload(), c);
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
