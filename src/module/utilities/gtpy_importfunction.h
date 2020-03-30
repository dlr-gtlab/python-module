/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_importfunction.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 23.03.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
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
  * @brief The stdout redirection class.
  */
typedef struct {
    PyObject_HEAD
    PythonQtObjectPtr defaultImp;
} GtpyMyImport;

#endif // GTPYIMPORTFUNCTION_H
