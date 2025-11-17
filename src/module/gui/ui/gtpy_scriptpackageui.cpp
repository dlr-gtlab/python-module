/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptpackageui.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_scriptpackageui.h"

#include <gt_icons.h>
#include <gt_datamodel.h>
#include <gt_filedialog.h>

#include "gt/resource/data/httpresource.h"
#include "gt/resource/data/directory.h"

GtpyScriptPackageUI::GtpyScriptPackageUI()
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
GtpyScriptPackageUI::icon(GtObject* obj) const
{
    return gt::gui::icon::folderOpen();
}
