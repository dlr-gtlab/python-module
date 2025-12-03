/* GTlab - Gas Turbine laboratory
 * Source File: scriptsui.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */
#ifndef SCRIPTSUI_H
#define SCRIPTSUI_H


#include <gt_objectui.h>

namespace gt
{

namespace resource
{

namespace gui
{

namespace ui
{

class ScriptsUI : public GtObjectUI
{
    Q_OBJECT

public:
    Q_INVOKABLE ScriptsUI();

    QIcon icon(GtObject* obj) const override;
};

} // namespace ui

} // namespace gui

} // namespace resource

} // namespace gt
#endif // SCRIPTSUI_H
