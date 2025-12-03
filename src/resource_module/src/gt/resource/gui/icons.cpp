/* GTlab - Gas Turbine laboratory
 * Source File: icons.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 20.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt/resource/gui/icons.h"

const QIcon&
gt::resource::gui::icon::fileMissing()
{
    static QIcon icon = gt::gui::getIcon(
        QStringLiteral(":/resourcemodule-icons/fileMissing.svg"));
    return icon;
}
