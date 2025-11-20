/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_directoryui.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 18.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_DIRECTORYUI_H
#define GTPY_DIRECTORYUI_H

#include <gt_objectui.h>

class GtpyDirectoryUI : public GtObjectUI
{
    Q_OBJECT

public:
    Q_INVOKABLE GtpyDirectoryUI();

    QIcon icon(GtObject* obj) const override;
};

#endif // GTPY_DIRECTORYUI_H
