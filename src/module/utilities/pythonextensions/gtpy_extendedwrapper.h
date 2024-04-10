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

#include <gt_object.h>

namespace GtpyExtendedWrapperModule
{
extern PyTypeObject GtpyExtendedWrapper_Type;

/// Python wrapper the the GTObject
struct GtpyExtendedWrapper
{
    PyObject_HEAD
    PythonQtInstanceWrapper* _obj;

    /// there are (rare) sitations, were you want to force the
    /// deletion of a object via python ownership, even when
    /// the object is part of a c++ tree
    bool forcePythonOwnership = {false};

    QObject* getObject() const;
};

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
