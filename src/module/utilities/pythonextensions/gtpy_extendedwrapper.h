/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_examplewrapper.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 18.02.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_EXAMPLEWRAPPER_H
#define GTPY_EXAMPLEWRAPPER_H

#include <QPointer>

#include "PythonQtPythonInclude.h"
#include "PythonQtInstanceWrapper.h"

#include "gtpy_globals.h"

namespace GtpyExtendedWrapperModule
{
extern PyTypeObject GtpyExtendedWrapper_Type;

typedef struct
{
    PyObject_HEAD
    PythonQtInstanceWrapper* _obj;

} GtpyExtendedWrapper;

#ifdef PY3K
static PyModuleDef
GtpyExtendedWrapper_Module =
{
    PyModuleDef_HEAD_INIT,
    "GtObjectWrapperModuleC",
    NULL,
    -1,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};
#endif
}

namespace GtpyCustomization
{
void customizeSlotCalling();
}


#endif // GTPY_EXAMPLEWRAPPER_H
