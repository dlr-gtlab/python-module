/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_createhelperfunction.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 26.02.2020
 * Author: Marvin Noethen (DLR AT-TWK)
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
