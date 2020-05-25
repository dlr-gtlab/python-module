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
typedef struct {
  PyObject_HEAD
  GtpyOutputChangedCB* callback;
  int softspace;
  bool closed;
} GtpyStdOutRedirect;

/// Namespace for static values to identify thread dict meta data.
namespace GtpyStdOut
{
    static const char* CONTEXT_KEY = "CONTEXT_NAME";
    static const char* OUTPUT_KEY = "OUTPUT_TO_CONSOLE";
    static const char* ERROR_KEY = "ERROR_TO_CONSOLE";
}


#endif // GTPY_STDOUT_H
