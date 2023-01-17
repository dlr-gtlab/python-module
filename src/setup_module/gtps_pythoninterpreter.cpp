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
#include "find_libpython.h"
#include <QTemporaryFile>

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
    return runPythonInterpreter(QStringList() << "-c" << pythonCommand, ok);
}

QString
GtpsPythonInterpreter::runScript(const QFile& scriptFile, bool* ok) const
{
    // Workaround, since QTemporaryFile().exists() is always false
    if (!QFile(scriptFile.fileName()).exists())
    {
        if (ok != nullptr)
        {
            *ok = false;
        }

        return {};
    }

    return runPythonInterpreter({scriptFile.fileName()}, ok);
}

QString
GtpsPythonInterpreter::runPythonInterpreter(const QStringList &args, bool *ok) const
{

    QProcess process;
    process.start(m_pythonExe, args);

    QString retval;
    bool succes{process.waitForFinished()};

    if (succes)
    {
        retval = QString(process.readAll());
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
GtpsPythonInterpreter::version() const
{
    return m_pyVersion;
}

const QString &GtpsPythonInterpreter::sharedLib() const
{
    return m_sharedLib;
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
                   "sys.stdout.write('%s' % sys.version_info.%1)"};

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

std::unique_ptr<QTemporaryFile>
getFindLibPythonScriptFile()
{
    auto file = std::make_unique<QTemporaryFile>();
    if (file->open())
    {
        file->write(findPythonLibCode);
        file->flush();
    }
    return file;
}

void
GtpsPythonInterpreter::setSharedLibPath()
{

    bool ok{false};
    auto sharedLib = runScript(*getFindLibPythonScriptFile(), &ok);

    if (ok)
    {
        m_sharedLib = sharedLib.replace("\\\\", "\\");
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
