/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_stdout.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 13.01.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_STDOUT_H
#define GTPY_STDOUT_H

#include "PythonQtPythonInclude.h"

#include "structmember.h"
#include <QString>

/**
 * @brief GtpyStdOutRedirect_Type
 * Type of the stdout redirection class
 */
extern PyTypeObject GtpyStdOutRedirect_Type;

/**
 * @brief Declares the callback that is called from the write() function in
 * Python.
 * @param contextName Name of the context in which the message was triggered.
 * @param output Determines whether the output should be shared or not.
 * @param error Determines whether the error message should be displayed or not.
 * @param message Triggered message.
 */
typedef void GtpyOutputChangedCB(const QString& contextName,
                                 const bool output, const bool error,
                                 const QString& message);

/**
  * @brief The stdout redirection class.
  */
typedef struct
{
    PyObject_HEAD
    GtpyOutputChangedCB* callback;
    int softspace;
    bool closed;
} GtpyStdOutRedirect;

#endif // GTPY_STDOUT_H
