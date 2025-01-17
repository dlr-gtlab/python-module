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
constexpr const char* GT_OBJECT_WRAPPER_MODULE_C = "GtObjectWrapperModuleC";

} // namespace module

namespace classes
{

constexpr const char* GTPY_EXTENDED_WRAPPER = "GtpyExtendedWrapper";

} // namespace classes

namespace funcs
{

constexpr const char* IMPORT_GT_CALCULATORS = "importGtCalculators";

} // namespace funcs

namespace attrs
{

constexpr const char* LOGGING_ENABLED = "__outputToAppConsole";

} // namespace attr

namespace import
{

QString defaultModules();

QString calculatorModule();

QString loggingFunctions();

} // namespace import

QString enableAppConsoleLogging(bool enable);

QString removeFromSysModules(const QString& moduleName);

} // namespace code

} // namespace gtpy

#endif // GTPYCODE_H
