/* GTlab - Gas Turbine laboratory
 * Source File: scriptsui.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "scriptsui.h"

#include <gt_icons.h>

namespace gt
{

namespace resource
{

namespace gui
{

namespace ui
{

ScriptsUI::ScriptsUI() { }

QIcon
ScriptsUI::icon(GtObject* obj) const
{
    return gt::gui::icon::fileCode();
}

} // namespace ui

} // namespace gui

} // namespace resource

} // namespace gt


