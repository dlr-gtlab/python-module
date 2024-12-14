/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_constants.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 29.03.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_CONSTANTS_H
#define GTPY_CONSTANTS_H

namespace gtpy
{
namespace constants
{
namespace modules
{
/// Python module names
constexpr const char* GT_CLASSES = "GtClasses";
constexpr const char* GT_OBJECT_WRAPPER_MODULE_C = "GtObjectWrapperModuleC";
constexpr const char* GT_LOGGING = "GtLogging";

} // namespace modules

namespace classes
{
/// Python class names
constexpr const char* GTPY_EXTENDED_WRAPPER = "GtpyExtendedWrapper";

} // namespace classes

namespace attr
{
/// Python attribute names
constexpr const char* LOG_TO_APP_CONSOLE = "__outputToAppConsole";
}

namespace funcs
{
/// Python function names
constexpr const char* IMPORT_GT_CALCS   = "importGtCalculators";
} // namespace func

/// Python function names
constexpr const char* CALL_SHARED_FUNC_F_NAME = "call_shared_function";
constexpr const char* SHARED_FUNC_F_NAME = "shared_function";
constexpr const char* SHARED_FUNC_IDS_F_NAME = "shared_function_ids";
constexpr const char* PROJECT_PATH_F_NAME = "projectPath";
constexpr const char* FOOTPRINT_F_NAME = "footprint";

/// Collection
constexpr const char* COLLECTION_ID = "Python Script Collection";
constexpr const char* COLLECTION_CAT = "category";
constexpr const char* COLLECTION_SUBCAT = "subcategory";

} // namespace constants

} // namespace gtpy

#endif // GTPY_CONSTANTS_H
