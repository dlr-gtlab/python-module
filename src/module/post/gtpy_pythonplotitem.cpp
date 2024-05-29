/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotitem.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 13.08.2019
 * Author: Stanislaus Reitenbach (AT-TW)
 */

#include "gtpy_pythonplotwidget.h"

#include "gtpy_pythonplotitem.h"

GtpyPythonPlotItem::GtpyPythonPlotItem()
{
    setObjectName("Python Processing");
}

GtAbstractPostWidget*
GtpyPythonPlotItem::createUI(GtPostTemplateItem* dm, QWidget* parent)
{
    GtpyPythonPlotItem* itm = qobject_cast<GtpyPythonPlotItem*>(dm);

    if (itm == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }

    return new GtpyPythonPlotWidget(itm, parent);
}
