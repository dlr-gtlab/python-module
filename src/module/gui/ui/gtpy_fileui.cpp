/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_fileui.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_fileui.h"

#include <gt_icons.h>

#include "gtpy_file.h"

#include "gtpy_scripteditormdi.h"

GtpyFileUI::GtpyFileUI()
{

}

QStringList
GtpyFileUI::openWith(GtObject* obj)
{
    QStringList list;

    if (auto* file = qobject_cast<GtpyFile*>(obj))
    {
        if (file->exists()) list << GT_CLASSNAME(GtpyScriptEditorMdi);
    }

    return list;
}
