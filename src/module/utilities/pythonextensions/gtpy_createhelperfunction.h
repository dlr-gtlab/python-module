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

/**
 * @brief Creates a new callable python object of the type
 * GtpyCreateHelperFunction_Type. When the python object is called, it creates
 * an instance of the helper class with the name helperName.
 * @param helperName Name of the helper class to be created by the Python
 * object.
 * @param self Parent of the new helper object.
 * @param module
 * @return New callable python object of the type GtpyCreateHelperFunction_Type
 */
PyObject* GtpyCreateHelperFunction_New(QString helperName, PyObject* self,
                                       PyObject* module);

//! defines a python object that creates a helper instance
typedef struct {
    PyObject_HEAD
    PyObject* m_helperName;
    PyObject* m_self;
    PyObject* m_module;
} GtpyCreateHelperFunctionObject;
#endif // GTPY_CREATEHELPERFUNCTION_H
