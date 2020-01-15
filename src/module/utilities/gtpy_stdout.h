/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_stdout.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 13.01.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_STDOUT_H
#define GTPY_STDOUT_H

#include "PythonQtPythonInclude.h"

#include "structmember.h"
#include <QString>

//! declares the type of the stdout redirection class
extern PyTypeObject GtpyStdOutRedirectType;

//! declares the callback that is called from the write() function
typedef void GtpyOutputChangedCB(const QString& contextName,
                                 const QString& message);

//! declares the stdout redirection class
typedef struct {
  PyObject_HEAD
  GtpyOutputChangedCB* _cb;
  int softspace;
  bool closed;
} GtpyStdOutRedirect;

#endif // GTPY_STDOUT_H
