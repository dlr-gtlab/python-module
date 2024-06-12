/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_importfunction.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 23.03.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYIMPORTFUNCTION_H
#define GTPYIMPORTFUNCTION_H

#include "PythonQt.h"
#include "PythonQtPythonInclude.h"

/**
 * @brief GtpyMyImport_Type
 * Type of the myImport class
 */
extern PyTypeObject GtpyMyImport_Type;

/**
  * @brief The import class.
  */
typedef struct {
    PyObject_HEAD
    PythonQtObjectPtr defaultImp;
} GtpyMyImport;

#endif // GTPYIMPORTFUNCTION_H
