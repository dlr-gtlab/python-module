/* GTlab - Gas Turbine laboratory
 * Source File: gtps_globals.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 30.11.2022
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QVector>
#include <QString>

#include "gt_globals.h"
#include "gt_moduleinterface.h"
#include "gt_settings.h"
#include "gtps_constants.h"


#include "gtps_globals.h"
#include <gt_application.h>


#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>

namespace
{
    QString pythonExe()
    {
#ifdef _WINDOWS
        return "python.exe";
#else
        return "bin/python";
#endif
    }
}

void
gtps::settings::setSetting(const QString& settingId, const QVariant& value)
{
    gtApp->settings()->setSetting(gtps::settings::path(settingId), value);
}

QVariant
gtps::settings::getSetting(const QString& settingId)
{
    return gtApp->settings()->getSetting(gtps::settings::path(settingId));
}


QString
gtps::apiVersionStr(const GtVersionNumber &version)
{
    return {QString::number(version.major()) + "." +
            QString::number(version.minor())};
}

QString
gtps::python::module::pythonModuleId(const GtVersionNumber& version)
{
    return QString{"Python Module (Python %1)"}
                   .arg(gtps::apiVersionStr(version));
}

QVector<GtVersionNumber>
gtps::python::version::supportedVersions()
{

    QVector<GtVersionNumber> available;

    QString moduleDir = QCoreApplication::applicationDirPath() + "/modules";

    static QRegularExpression pymodRe("(lib)?(?i)(GTlabPython)([0-9])([0-9]+)(-d)?.(dll|so|dylib)$");

    QDirIterator it(moduleDir, QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QString filePath = it.next();
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();

        auto matcher = pymodRe.match(fileName);

        if (matcher.hasMatch())
        {
            available.push_back(GtVersionNumber(matcher.captured(3).toInt(),
                                                matcher.captured(4).toInt()));
        }
    }

    std::sort(available.begin(), available.end());

    return available;
}

bool
gtps::python::version::isSupported(const GtVersionNumber& version)
{
    auto versions = supportedVersions();
    auto iter = std::find_if(versions.begin(), versions.end(),
                    [&version] (const GtVersionNumber& supported) {
                        return supported.major() == version.major() &&
                                supported.minor() == version.minor();});
    return iter != versions.end() ? true : false;
}

QString
gtps::settings::path(const QString& settingId)
{
    return moduleSettingPath(GT_MODULENAME(), settingId);
}

QString
gtps::embeddedPythonPath()
{
    // test, if there is an embedded python distribution
    auto pythonDir = QDir(QCoreApplication::applicationDirPath()
                          + "/../lib/python");

    QString pythonPath = pythonDir.absoluteFilePath(pythonExe());

    if (!QFile(pythonPath).exists()) return {};

    return pythonPath;
}

bool
gtps::settings::useEmbeddedPython()
{
    QVariant useEmbeddedVar = getSetting(gtps::constants::USE_EMBEDDED_S_ID);

    bool useEmbedded = true;

    if (useEmbeddedVar.isValid()) useEmbedded = useEmbeddedVar.toBool();

    return useEmbedded && !gtps::embeddedPythonPath().isEmpty();
}

QString
gtps::pythonPath()
{
    if (gtps::settings::useEmbeddedPython())
    {
        return gtps::embeddedPythonPath();
    }
    else
    {
        return gtps::settings::customPythonPath();
    }
}

QString
gtps::settings::customPythonPath()
{
    return getSetting(gtps::constants::PYEXE_S_ID).toString();
}
