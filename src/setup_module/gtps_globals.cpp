/* GTlab - Gas Turbine laboratory
 * Source File: gtps_globals.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.11.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QVector>
#include <QString>

#include "gtps_globals.h"

QVector<GtVersionNumber>
gtps::supportedVersions()
{
    return QVector<GtVersionNumber>{GtVersionNumber(3, 9)};
}

QString
gtps::apiVersionStr(const GtVersionNumber& version)
{
    return {QString::number(version.major()) + "." +
            QString::number(version.minor())};
}

QString
gtps::pythonModuleId(const GtVersionNumber& version)
{
    return QString{"Python Module (Python %1)"}
                   .arg(apiVersionStr(version));
}

bool
gtps::validation::isSupported(const GtVersionNumber& version)
{
    auto versions = supportedVersions();
    auto iter = std::find_if(versions.begin(), versions.end(),
                    [&version] (const GtVersionNumber& supported) {
                        return supported.major() == version.major() &&
                                supported.minor() == version.minor();});
    return iter != versions.end() ? true : false;
}
