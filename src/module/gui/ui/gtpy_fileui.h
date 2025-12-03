/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_fileui.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_FILEUI_H
#define GTPY_FILEUI_H

#include <gt/resource/gui/ui/fileui.h>

class GtpyFileUI : public gt::resource::gui::ui::FileUI
{
    Q_OBJECT

public:
    Q_INVOKABLE GtpyFileUI();

    QStringList openWith(GtObject* obj) override;
};
#endif // GTPY_FILEUI_H
