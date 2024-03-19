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

#define QSTRING_TO_CHAR_PTR(x) QString(x).toLocal8Bit().data()
#define QSTRING_AS_PYSTRING(x) PyString_FromString(QSTRING_TO_CHAR_PTR(x))

#define MODULE_GTCALCULATORS "GtCalculators"

#define PyObjectAPIReturn PyObject*

namespace GtpyGlobals
{
///Environment Variables
extern const QString PYTHONHOME_VAR;

///Python Modules
extern const QString MODULE_GtObjectWrapperModuleC;
extern const QString MODULE_GtCalculators;
extern const QString MODULE_GtLogging;
extern const QString MODULE_GtLogging_C;

///Python Classes
extern const QString GTOBJECT_WRAPPER;
extern const QString CLASS_GtpyPyLogger;

///Python Attribute
extern const QString ATTR_task;
extern const QString ATTR_outputToApp;

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

/// Collection
extern const QString COLLECTION_ID;
extern const QString COLLECTION_cat;
extern const QString COLLECTION_subcat;
}
#endif // GTPYGLOBALS_H
