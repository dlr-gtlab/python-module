/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_code.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 10.01.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYCODE_H
#define GTPYCODE_H

#include <QString>

namespace gtpy
{
namespace code
{
namespace modules
{

// PythonQt
constexpr const char* PYTHON_QT = "PythonQt";

// PythonQt submodules
constexpr const char* QTCORE = "QtCore";
constexpr const char* GT_CLASSES = "GtClasses";

// custom modules
constexpr const char* GT_LOGGING = "GtLogging";
constexpr const char* GT_OBJECT_WRAPPER_MODULE = "GtObjectWrapperModuleC";
constexpr const char* GT_CALCULATORS = "GtCalculators";

} /// namespace modules

namespace classes
{

constexpr const char* GTPY_EXTENDED_WRAPPER = "GtpyExtendedWrapper";

} /// namespace classes

namespace funcs
{

// shared functions api
constexpr const char* CALL_SHARED_FUNC_F_NAME = "call_shared_function";
constexpr const char* SHARED_FUNC_F_NAME = "shared_function";
constexpr const char* SHARED_FUNC_IDS_F_NAME = "shared_function_ids";

// predefined functions
constexpr const char* PROJECT_PATH_F_NAME = "projectPath";
constexpr const char* FOOTPRINT_F_NAME = "footprint";
constexpr const char* ENV_VARS_F_NAME = "envVars";
constexpr const char* IMPORT_GT_CALCULATORS = "importGtCalculators";

// logging functions
constexpr const char* GT_DEBUG = "gtDebug";
constexpr const char* GT_INFO = "gtInfo";
constexpr const char* GT_WARNING = "gtWarning";
constexpr const char* GT_ERROR = "gtError";
constexpr const char* GT_FATAL = "gtFatal";

} /// namespace funcs

namespace attrs
{

constexpr const char* LOGGING_ENABLED = "__outputToAppConsole";
constexpr const char* TASK = "__task";

} /// namespace attrs


namespace keys
{

constexpr const char* CONTEXT = "CONTEXT_NAME";
constexpr const char* OUTPUT = "OUTPUT_TO_CONSOLE";
constexpr const char* ERROR = "ERROR_TO_CONSOLE";

} /// namespace keys

namespace import
{

/**
 * @brief Returns the Python import statements for essential modules and
 * classes that enable efficient use of the GTlab Python integration.
 *
 * The import statements are:
 * - from GtObjectWrapperModuleC import GtpyExtendedWrapper
 * - from PythonQt import GtClasses
 * - from PythonQt import QtCore
 *
 * @return The default Python import statements.
 */
QString defaultImports();

/**
 * @brief Returns the Python code to import the GtCalculator classes and the
 * findGtTask(name: str) function. This code allows a GtpyModule or GtpyContext
 * to interact with GTlab process data and to create a process chain using
 * Python code.
 *
 * @return The Python code to import the GtCalculator classes and the
 * findGtTask(name: str) function.
 */
QString calculatorModule();

/**
 * @brief Returns the Python code to import GTlab logging functions.
 *
 * The generated import statement:
 *
 * from GtLogging import gtDebug, gtInfo, gtWarning, gtError, gtFatal
 *
 * @return The Python code to import GTlab logging functions.
 */
QString loggingFunctions();

} /// namespace import

/**
 * @brief Generates Python code to enable or disable logging to the application
 * console.
 *
 * The generated code sets the logging flag as follows:
 *
 * __outputToAppConsole = True # or False, based on the passed parameter
 *
 * @param enable If true, the code to enable logging to the application
 * console is returned. If false, the code to disable logging is returned.
 * @return The Python code to enable or disable logging to the application
 * console.
 */
QString enableAppConsoleLogging(bool enable);

/**
 * @brief Returns Python code that defines functions for interacting with
 * GTlab. The GTlab Python identifier must be passed to these functions.
 *
 * Ensure that the gtApp instance is added and available in the GtpyModule
 * or GtpyContext using the passed Python identifier before evaluating this
 * code.
 *
 * The generated functions are:
 *  - openProject(projectName: str) -> Optional[GtpyExtendedWrapper]
 *  - currentProject() -> Optional[GtpyExtendedWrapper]
 *  - init(id: str = '')
 *  - switchSession(id: str = '')
 *
 * @param gtlabPyIdent The Python identifier for the GTlab application object.
 * @return The Python code defining functions to interact with GTlab.
 */
QString enableGTlabControl(const QString& gtlabPyIdent);

} /// namespace code

} /// namespace gtpy

#endif // GTPYCODE_H
