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

#include "gt_version.h"

#include <Python.h>

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_propertystructcontainer.h"
#endif

namespace gtpy {

namespace convert {

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
PyObject*
fromPropertyStructContainer(const GtPropertyStructContainer& con);
#endif

} // namespace convert

} // namespace gtpy



#endif // GTPYCONVERT_H
