/* GTlab - Gas Turbine laboratory
 * Source File: gtps_pythonevaluator.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 20.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QProcess>

#include "gt_logging.h"

#include "gtps_pythonevaluator.h"

GtpsPythonEvaluator::GtpsPythonEvaluator(const QString& pythonExe) :
        m_pythonExe{pythonExe}
{
    setPythonVersion();
}

QString
GtpsPythonEvaluator::eval(const QString& pythonCommand, bool* ok) const
{
    QProcess process;
    process.start(m_pythonExe, QStringList() << "-c" << pythonCommand);

    QString retval;
    auto succes = process.waitForFinished();

    if (succes)
    {
        retval = process.readAll();
    }
    else
    {
        retval = process.errorString();
    }

    if (ok)
    {
        *ok = succes;
    }

    return retval;
}

bool
GtpsPythonEvaluator::isValid() const
{

    bool ok{false};
    eval("import sys", &ok);
    return ok;
}

QString
GtpsPythonEvaluator::pythonExe() const
{
    return m_pythonExe;
}

GtVersionNumber
GtpsPythonEvaluator::pythonVersion() const
{
    return m_pyVersion;
}

void
GtpsPythonEvaluator::setPythonVersion()
{
    auto pyCode = QString("import sys;"
                          "print('%s' % sys.version_info.%1, end='')");
    bool majorOk{false};
    bool minorOk{false};
    bool patchOk{false};

    QString major = eval(pyCode.arg("major"), &majorOk);
    QString minor = eval(pyCode.arg("minor"), &minorOk);
    QString patch = eval(pyCode.arg("micro"), &patchOk);

    if (majorOk && minorOk && patchOk)
    {
        m_pyVersion = GtVersionNumber{major.toInt(), minor.toInt(),
                                        patch.toInt()};
    }
}
