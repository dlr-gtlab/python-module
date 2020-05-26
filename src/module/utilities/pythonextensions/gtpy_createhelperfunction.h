/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_createhelperfunction.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 26.02.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_CREATEHELPERFUNCTION_H
#define GTPY_CREATEHELPERFUNCTION_H

#include <QChar>

#include "PythonQtPythonInclude.h"

/**
 * @brief GtpyCreateHelperFunction_Type
 */
extern PyTypeObject GtpyCreateHelperFunction_Type;

//! defines a python object that creates a helper instance
typedef struct
{
    PyObject_HEAD
    PyObject* m_helperName;
    PyObject* m_self;
} GtpyCreateHelperFunctionObject;
#endif // GTPY_CREATEHELPERFUNCTION_H
