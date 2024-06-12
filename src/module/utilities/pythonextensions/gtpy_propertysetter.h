/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_propertysetter.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 26.02.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_PROPERTYSETTER_H
#define GTPY_PROPERTYSETTER_H

#include <QChar>

#include "PythonQtPythonInclude.h"

/**
 * @brief This helper type used for to make function calls lile
 * `myObject->setMyProperty(value)` work.
 *
 * The extended wrapper creates a temporary instance of this type,
 * which basically wrap the call setXXX.
 */
extern PyTypeObject GtpyPropertySetter_Type;

PyObject*
GtpyPropertySetter_New(const QString& propId, PyObject* self,
                                 PyObject* module);

//! defines a callable python object that set GtProperties
typedef struct {
    PyObject_HEAD
    PyObject* m_propId;
    PyObject* m_self;
    PyObject* m_module;
} GtpyPropertySetterObject;

#endif // GTPY_PROPERTYSETTER_H
