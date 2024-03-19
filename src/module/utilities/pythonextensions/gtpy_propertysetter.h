/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_propertysetter.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 26.02.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
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
