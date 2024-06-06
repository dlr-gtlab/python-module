/* GTlab - Gas Turbine laboratory
 * Source File: gtps_systemsettings.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 18.01.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt_environment.h"

#include "gtps_systemsettings.h"

QString
gtps::system::pathSeperator()
{
#ifdef _WIN32
    return ";";
#else
    return ":";
#endif
}

void
gtps::system::clearPythonVars()
{
    qunsetenv("PYTHONPATH");
    qunsetenv("PYTHONHOME");
}

QByteArray
gtps::system::pythonPath()
{
#ifdef _WIN32
    return qEnvironmentVariable("PYTHONPATH").toUtf8();
#else
    return qgetenv("PYTHONPATH");
#endif
}

void
gtps::system::setPythonPath(const QByteArray& val)
{
    qputenv("PYTHONPATH", val);
}

QByteArray
gtps::system::pythonHome()
{
#ifdef _WIN32
    return qEnvironmentVariable("PYTHONHOME").toUtf8();
#else
    return qgetenv("PYTHONHOME");
#endif
}

void
gtps::system::setPythonHome(const QByteArray& val)
{
    qputenv("PYTHONHOME", val);
    setGtlabPythonHome(val);
}

void
gtps::system::setGtlabPythonHome(const QByteArray &val)
{
    auto var = gtEnvironment->value("PYTHONHOME");

    if (!var.isNull())
    {
        if (var.toByteArray() != val)
        {
            gtEnvironment->setValue("PYTHONHOME", QString{val});
            gtEnvironment->saveEnvironment();
        }
    }
}
