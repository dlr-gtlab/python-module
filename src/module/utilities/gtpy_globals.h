/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_globals.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 11.05.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYGLOBALS_H
#define GTPYGLOBALS_H

#include <QString>

#define QSTRING_TO_CHAR_PTR(x) x.toStdString().data()
#define QSTRING_AS_PYSTRING(x) PyString_FromString(QSTRING_TO_CHAR_PTR(x))

namespace GtpyGlobals
{
///Python Modules
extern const QString GTOBJECT_WRAPPER_MODULE;
extern const QString MODULE_GtCalculators;
extern const QString MODULE_GtLogging;

///Python Classes
extern const QString GTOBJECT_WRAPPER;

///Python Attribute
extern const QString ATTR_task;

///Python Functions
extern const QString FUNC_importGtCalc;
extern const QString FUNC_currentProPath;

///StdOut meta data
extern const char* CONTEXT_KEY;
extern const char* OUTPUT_KEY;
extern const char* ERROR_KEY;

struct StdOutMetaData
{
    QString contextName = QString();
    bool output = false;
    bool error = false;
};

}
#endif // GTPYGLOBALS_H
