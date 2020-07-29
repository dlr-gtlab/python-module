/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_globals.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 11.05.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_globals.h"

namespace GtpyGlobals
{
///Environment Variables
const QString PYTHONHOME_VAR = "PYTHONHOME";

///Python Modules
const QString GTOBJECT_WRAPPER_MODULE = "GtObjectWrapperModuleC";
const QString MODULE_GtCalculators    = "GtCalculators";
const QString MODULE_GtLogging        = "GtLogging";
const QString MODULE_GtLogging_C      = "GtLoggingC";

///Python Classes
const QString GTOBJECT_WRAPPER   = "GtpyExtendedWrapper";
const QString CLASS_GtpyPyLogger = "GtpyPyLogger";

///Python Attribute
const QString ATTR_task = "__task";

///Python Functions
const QString FUNC_importGtCalc   = "importGtCalculators";
const QString FUNC_currentProPath = "projectPath";

///StdOut meta data
const char* CONTEXT_KEY = "CONTEXT_NAME";
const char* OUTPUT_KEY  = "OUTPUT_TO_CONSOLE";
const char* ERROR_KEY   = "ERROR_TO_CONSOLE";
}
