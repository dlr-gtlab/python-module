/* GTlab - Gas Turbine laboratory
 * Source File: ResourcesModule
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 04.11.2025
 * Author: Marvin Nöthen (DLR AT-TWK)
 */

#ifndef FILEUI_H
#define FILEUI_H

#include <gt_objectui.h>

namespace gt
{

namespace resource
{

namespace gui
{

namespace ui
{

class FileUI : public GtObjectUI
{
    Q_OBJECT

public:
    Q_INVOKABLE FileUI();

    QIcon icon(GtObject* obj) const override;

    QStringList openWith(GtObject* obj) override;

    QRegExp validatorRegExp() override;

protected:
    QVariant specificData(GtObject* obj, int role, int column) const override;
};

} // namespace ui

} // namespace gui

} // namespace resource

} // namespace gt


#endif // FILEUI_H
