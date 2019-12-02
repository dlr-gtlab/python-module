/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotdata.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 13.08.2019
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
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
GtpyPythonPlotData::setScript(QString& script)
{
    script.replace("\n", "\r");

    m_script = script;
}
