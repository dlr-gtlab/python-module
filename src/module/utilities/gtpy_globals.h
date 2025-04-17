/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_globals.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 11.05.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYGLOBALS_H
#define GTPYGLOBALS_H


#define PyObjectAPIReturn PyObject*

#define gtDevMode() (gtApp && gtApp->devMode())

namespace gtpy
{

namespace constants
{

    /// Collection
    constexpr const char* COLLECTION_ID = "Python Script Collection";
    constexpr const char* COLLECTION_CAT = "category";
    constexpr const char* COLLECTION_SUBCAT = "subcategory";

} // namespace constants

} // namespace gtpy


#endif // GTPYGLOBALS_H
