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
///Python function names
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
