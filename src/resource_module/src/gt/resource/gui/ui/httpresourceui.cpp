/* GTlab - Gas Turbine laboratory
 * Source File: httpresourceui.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "httpresourceui.h"

#include <gt_logging.h>

#include <gt_icons.h>

#include "gt/resource/data/httpresource.h"

namespace gt
{

namespace resource
{

namespace gui
{

namespace ui
{

HttpResourceUI::HttpResourceUI()
{
    using HttpResource = gt::resource::data::HttpResource;

    addSingleAction(tr("Test Connection"), [](GtObject* obj) {
        auto* res = qobject_cast<HttpResource*>(obj);
        if (!res || !res->exists())
        {
            gtError() << "Connection failed.";
        }
        else
        {
            gtInfo() << "Connection successful.";
        }
    }).setIcon(gt::gui::icon::connection());

    addSingleAction(tr("GET"), [](GtObject* obj) {
        auto* res = qobject_cast<HttpResource*>(obj);
        if (!res || !res->exists())
        {
            gtError() << "Connection failed.";
            return;
        }

        gtInfo() << "Fetching content from: " << res->url().toString();

        auto data = res->readAll();
        if (data.isEmpty())
        {
            gtWarning() << "No content received or request failed.";
        }
        else
        {
            gtInfo() << "Received content (first 200 bytes):\n" << data.left(200);
        }
    }).setIcon(gt::gui::icon::download());
}

QIcon
HttpResourceUI::icon(GtObject *obj) const
{
    return gt::gui::icon::web();
}

} // namespace ui

} // namespace gui

} // namespace resource

} // namespace gt
