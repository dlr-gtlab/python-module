/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_convert.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 15.11.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCONVERT_H
#define GTPYCONVERT_H

#include "gt_globals.h"

#include <Python.h>
#include "gtpypp.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_propertystructcontainer.h"
#endif

namespace gtpy {

namespace convert {

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
/**
 * @brief Converts the passed GtPropertyStructContainer into a Python dict
 * and returns it. The entries in the GtPropertyStructContainer require a
 * member named 'name' and a member named 'value'. In Python dict, the name
 * is used as a key to identify the corresponding value. The dict is an
 * independent instance and changes to it do not affect
 * the GtPropertyStructContainer.
 * @param con GtPropertyStructContainer to convert.
 * @return Python dict with name and value pairs from the passed
 * GtPropertyStructContainer.
 */
PyPPObject
fromPropertyStructContainer(const GtPropertyStructContainer& con);
#endif

} // namespace convert

} // namespace gtpy



#endif // GTPYCONVERT_H
