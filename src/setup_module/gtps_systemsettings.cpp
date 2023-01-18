/* GTlab - Gas Turbine laboratory
 * Source File: gtps_systemsettings.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 18.01.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

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
}
