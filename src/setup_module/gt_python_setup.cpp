/* GTlab - Gas Turbine laboratory
 * Source File: gt_python_setup.cpp
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.10.2022
 *  Author: Marvin Nöthen (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <QDir>
#include <QProcess>

#include "gt_logging.h"
#include "gt_environment.h"

#include "gtps_pythonevaluator.h"

#include "gt_python_setup.h"

#if GT_VERSION >= 0x010700
GtVersionNumber
GtPythonSetupModule::version()
{
    return GtVersionNumber(1, 0, 0);
}
#else
int
GtPythonSetupModule::version()
{
    return 1;
}
#endif

QString
GtPythonSetupModule::description() const
{
    return QStringLiteral("GTlab Python Setup Module");
}

void
GtPythonSetupModule::onLoad()
{
    auto pyExe = gtEnvironment->value("PYTHONHOME").toString();
    pyExe = QDir::toNativeSeparators(pyExe);

    if (!pyExe.endsWith(QDir::separator()))
        pyExe += QDir::separator();

    pyExe += "python.exe";

    GtpsPythonEvaluator evaluator{pyExe};

    if (!evaluator.isValid())
    {
        qDebug() << "add suppression";
        gtApp->addSuppression(*this, "Python Module");
    }
}
