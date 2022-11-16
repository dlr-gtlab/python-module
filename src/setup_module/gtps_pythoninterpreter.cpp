/* GTlab - Gas Turbine laboratory
 * Source File: gtps_pythoninterpreter.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 20.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QProcess>
#include <QDir>
#include <QFileInfo>

#include "gt_logging.h"

#include "gtps_pythoninterpreter.h"

GtpsPythonInterpreter::GtpsPythonInterpreter(const QString& pythonExe) :
        m_pythonExe{pythonExe}
{
    setPythonVersion();
    setSharedLibPath();
    setSysPaths();
}

QString
GtpsPythonInterpreter::eval(const QString& pythonCommand, bool* ok) const
{
    QProcess process;
    process.start(m_pythonExe, QStringList() << "-c" << pythonCommand);

    QString retval;
    bool succes{process.waitForFinished()};

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
GtpsPythonInterpreter::isValid() const
{

    bool ok{false};
    eval("import sys", &ok);
    return ok;
}

const QString&
GtpsPythonInterpreter::pythonExe() const
{
    return m_pythonExe;
}

const GtVersionNumber&
GtpsPythonInterpreter::pythonVersion() const
{
    return m_pyVersion;
}

const QString&
GtpsPythonInterpreter::sharedLibPath() const
{
    return m_sharedLibPath;
}

const QStringList&
GtpsPythonInterpreter::sysPaths() const
{
    return m_sysPaths;
}

QString
GtpsPythonInterpreter::pythonHomePath() const
{
    return QDir::toNativeSeparators(QFileInfo(m_pythonExe).absolutePath());
}

void
GtpsPythonInterpreter::setPythonVersion()
{
    QString pyCode{"import sys;"
                   "print('%s' % sys.version_info.%1, end='')"};

    bool majorOk{false};
    bool minorOk{false};
    bool patchOk{false};

    auto major = eval(pyCode.arg("major"), &majorOk);
    auto minor = eval(pyCode.arg("minor"), &minorOk);
    auto patch = eval(pyCode.arg("micro"), &patchOk);

    if (majorOk && minorOk && patchOk)
    {
        m_pyVersion = GtVersionNumber{major.toInt(), minor.toInt(),
                                        patch.toInt()};
    }
}

void
GtpsPythonInterpreter::setSharedLibPath()
{
    QString pyCode{
    "import os;"
    "import sys;"
    "pyDll = 'python%s%s.dll'%(sys.version_info.major, sys.version_info.minor);"
    "[exit(print(p, end='')) for p in sys.path "
    "if os.path.isfile(os.path.join(p, pyDll))];"};

    bool ok{false};
    auto homePath = eval(pyCode, &ok);

    if (ok)
    {
        m_sharedLibPath = homePath.replace("\\\\", "\\");
    }
}

void
GtpsPythonInterpreter::setSysPaths()
{
    QString pyCode{"import sys;print(', '.join([x for x in sys.path if x]), "
                   "end='')"};
    bool ok{false};
    auto pyPaths = eval(pyCode, &ok);
    if (ok)
    {
        m_sysPaths = pyPaths.split(", ");
        auto toNativeSep = [](QString& pyPath){
                            pyPath = QDir::toNativeSeparators(pyPath);};
        std::for_each(m_sysPaths.begin(), m_sysPaths.end(), toNativeSep);
    }
}
