/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_projectpathfunction.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 03.06.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYPROJECTPATHFUNCTION_H
#define GTPYPROJECTPATHFUNCTION_H


#include "PythonQtPythonInclude.h"

/**
 * GtpyProjectPathFunction namespace
 */
namespace GtpyProjectPathFunction
{

/**
 * @brief GtpyProjectPathFunction_Type
 */
extern PyTypeObject GtpyProjectPathFunction_Type;

// Python Object
typedef struct
{
    PyObject_HEAD

} GtpyProjectPathFunctionObject;

}

#endif // GTPYPROJECTPATHFUNCTION_H
