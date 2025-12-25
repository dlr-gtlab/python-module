/* GTlab - Gas Turbine laboratory
 * Source File: ResourcesModule
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 04.11.2025
 * Author: Marvin Nöthen (DLR AT-TWK)
 */

#ifndef DIRECTORYUI_H
#define DIRECTORYUI_H

#include <gt_objectui.h>

namespace gt
{

namespace resource
{

namespace gui
{

namespace ui
{

class DirectoryUI : public GtObjectUI
{
    Q_OBJECT

public:
    Q_INVOKABLE DirectoryUI();

    QIcon icon(GtObject* obj) const override;

    QRegExp validatorRegExp() override;

    void doubleClicked(GtObject* obj) override;

protected:
    QVariant specificData(GtObject* obj, int role, int col) const override;

};

} // namespace ui

} // namespace gui

} // namespace resource

} // namespace gt

#endif // DIRECTORYUI_H
