/* GTlab - Gas Turbine laboratory
 * Source File: httpresourceui.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef HTTPRESOURCEUI_H
#define HTTPRESOURCEUI_H

#include <gt_objectui.h>

namespace gt
{

namespace resource
{

namespace gui
{

namespace ui
{

class HttpResourceUI : public GtObjectUI
{
    Q_OBJECT

public:
    Q_INVOKABLE HttpResourceUI();

    QIcon icon(GtObject* obj) const override;
};

} // namespace ui

} // namespace gui

} // namespace resource

} // namespace gt

#endif // HTTPRESOURCEUI_H
