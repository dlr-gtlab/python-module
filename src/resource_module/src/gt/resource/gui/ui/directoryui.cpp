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
#include <QFileInfo>
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
        auto* res = qobject_cast<Directory*>(obj);
        return res ? res->isOpen() : false;
    };

    addSingleAction(tr("Open Directory"), [](GtObject* obj) {
        auto* res = qobject_cast<Directory*>(obj);
        if (res) res->open();
    })
        .setIcon(gt::gui::icon::file())
        .setVerificationMethod([isOpen](GtObject* obj){ return !isOpen(obj); });

    addSingleAction(tr("Close Directory"), [](GtObject* obj) {
        auto* res = qobject_cast<Directory*>(obj);
        if (res) res->close();
    })
        .setIcon(gt::gui::icon::file())
        .setVerificationMethod(isOpen);

    addSeparator();

    auto dirExists = [](GtObject* obj) {
        auto* res = qobject_cast<Directory*>(obj);
        if (!res) return false;
        return res->exists();
    };

    addSingleAction(tr("Show in Explorer"),  [](GtObject* obj) {
        auto* res = qobject_cast<Directory*>(obj);
        if (!res) return;

        QDir dir{res->url().toLocalFile()};
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir.absolutePath()));
    })
        .setIcon(gt::gui::icon::folderOpen()).
        setVerificationMethod(dirExists);
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
