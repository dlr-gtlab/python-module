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
    return QVector<GtVersionNumber>{GtVersionNumber(3, 9)};
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
