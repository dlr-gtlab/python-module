/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotdata.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 13.08.2019
 * Author: Stanislaus Reitenbach (DLR AT-TWK)
 */

#include "gtpy_pythonplotdata.h"

GtpyPythonPlotData::GtpyPythonPlotData()
{

}

QString
GtpyPythonPlotData::script() const
{
    return m_script;
}

void
GtpyPythonPlotData::setScript(const QString& script)
{
    m_script = script;

#if GT_VERSION >= 0x010700

#else
    m_script.replace("\n", "\r");
#endif

}
