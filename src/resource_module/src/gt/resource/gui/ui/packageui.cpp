/* GTlab - Gas Turbine laboratory
 * Source File: packageui.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "packageui.h"

#include <gt_icons.h>
#include <gt_datamodel.h>
#include <gt_filedialog.h>

#include "gt/resource/data/httpresource.h"
#include "gt/resource/data/directory.h"

namespace gt
{

namespace resource
{

namespace gui
{

namespace ui
{

PackageUI::PackageUI()
{
    addSingleAction(tr("Create Directory Resource"), [](GtObject* obj) {
        QString dirPath = GtFileDialog::getExistingDirectory(
            nullptr, tr("Choose Directory"));

        if (dirPath.isEmpty()) return;

        auto* res = new gt::resource::data::Directory{dirPath};
        res->setObjectName("Directory");
        gtDataModel->appendChild(res, obj);

    }).setIcon(gt::gui::icon::folder());

    addSingleAction(tr("Create HTTP Resource"), [](GtObject* obj) {
        auto* res = new gt::resource::data::HttpResource{};
        res->setObjectName("HttpResource");
        gtDataModel->appendChild(res, obj);
    }).setIcon(gt::gui::icon::web());
}

QIcon
PackageUI::icon(GtObject* obj) const
{
    return gt::gui::icon::data();
}

} // namespace ui

} // namespace gui

} // namespace resource

} // namespace gt
