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

#include "gtps_globals.h"

#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>

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

    static QRegularExpression pymodRe("(lib)?GTlabPython([0-9])([0-9]+).(dll|so|dylib)$");

    QDirIterator it(moduleDir, QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QString filePath = it.next();
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();

        auto matcher = pymodRe.match(fileName);

        if (matcher.hasMatch())
        {
            available.push_back(GtVersionNumber(matcher.captured(2).toInt(),
                                                matcher.captured(3).toInt()));
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
