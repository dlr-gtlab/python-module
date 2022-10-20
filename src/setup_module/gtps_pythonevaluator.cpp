/* GTlab - Gas Turbine laboratory
 * Source File: gtps_pythonevaluator.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 20.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QProcess>
#include "gtps_pythonevaluator.h"

GtpsPythonEvaluator::GtpsPythonEvaluator(const QString& pythonExe) :
        m_pythonExe{pythonExe}
{
    evaluate("import sys", &m_isValid);
}

QString
GtpsPythonEvaluator::evaluate(const QString& pythonCommand, bool* ok) const
{
    QProcess process;
    process.start(m_pythonExe, QStringList() << "-c" << pythonCommand);

    QString retval;
    auto succes = process.waitForFinished();

    if (succes)
        retval = process.readAll();
    else
        retval = process.errorString();

    if (ok)
        *ok = succes;

    return retval;
}

bool
GtpsPythonEvaluator::isValid() const
{
    return m_isValid;
}
