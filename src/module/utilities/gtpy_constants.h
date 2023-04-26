/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_constants.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 29.03.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_CONSTANTS_H
#define GTPY_CONSTANTS_H

namespace gtpy
{

namespace constants
{

///Python function names
constexpr const char* CALL_SHARED_FUNC_F_NAME = "call_shared_function";
constexpr const char* SHARED_FUNC_F_NAME = "shared_function";
constexpr const char* SHARED_FUNC_IDS_F_NAME = "shared_function_ids";
constexpr const char* PROJECT_PATH_F_NAME = "projectPath";

/// Collection
constexpr const char* COLLECTION_ID = "Python Script Collection";
constexpr const char* COLLECTION_CAT = "category";
constexpr const char* COLLECTION_SUBCAT = "subcategory";

} // namespace constants

} // namespace gtpy

#endif // GTPY_CONSTANTS_H
