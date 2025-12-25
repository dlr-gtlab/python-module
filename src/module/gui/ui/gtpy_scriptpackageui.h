/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptpackageui.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_SCRIPTPACKAGEUI_H
#define GTPY_SCRIPTPACKAGEUI_H

#include <gt_objectui.h>

class GtpyScriptPackageUI : public GtObjectUI
{
    Q_OBJECT

public:
    Q_INVOKABLE GtpyScriptPackageUI();

    QIcon icon(GtObject* obj) const override;
};

#endif // GTPY_SCRIPTPACKAGEUI_H
