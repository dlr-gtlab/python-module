/* GTlab - Gas Turbine laboratory
 * Source File: packageui.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef PACKAGEUI_H
#define PACKAGEUI_H

#include <gt_objectui.h>

namespace gt
{

namespace resource
{

namespace gui
{

namespace ui
{


class PackageUI : public GtObjectUI
{
    Q_OBJECT

public:
    Q_INVOKABLE PackageUI();

    QIcon icon(GtObject* obj) const override;

};

} // namespace ui

} // namespace gui

} // namespace resource

} // namespace gt

#endif // PACKAGEUI_H
