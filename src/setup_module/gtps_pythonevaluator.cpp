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
    bool ok{false};
    evaluate("import sys", &ok);
    return ok;
}

QString
GtpsPythonEvaluator::pythonVersion() const
{
    auto pyCode = "import sys;print('%s.%s.%s' % (sys.version_info.major, "
                  "sys.version_info.minor, sys.version_info.micro), end='')";

    bool ok{false};
    auto version = evaluate(pyCode, &ok);
    if (!ok)
        return {};

    return version;
}
