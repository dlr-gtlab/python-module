/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_threadsupport.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.12.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_THREADSUPPORT_H
#define GTPY_THREADSUPPORT_H

#include "PythonQtPythonInclude.h"
#include "gt_pythonmodule_exports.h"

#include <QObject>

#define GTPY_GIL_SCOPE GtpyGilScope internal_gilscope;

/**
 * @brief The GtpyGilScope class
 */
class GT_PYTHON_EXPORT GtpyGilScope : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief The GtpyGilScope constructor.
     */
    GtpyGilScope();

    /**
     * @brief Destructor releases the GIL.
     */
    ~GtpyGilScope();

    /**
     * @brief Sets m_enableGILScope to the specified flag.
     * @param flage True if the GIL scope should be enabled.
     */
    static void setGILScopeEnabled(bool flag);

    /**
     * @brief Returns whether the GIL scope is enabled or not.
     * @return Whether the GIL scope is enabled or not.
     */
    static bool isGILScopeEnabled();

private slots:
    /**
     * @brief Releases the GIL.
     */
    void release();

private:
    /// GIL state
    PyGILState_STATE m_state;

    /// GIL ensured
    bool m_ensured;

    /// GIL scope is enabled
    static bool m_enableGILScope;
};

#endif // GTPY_THREADSUPPORT_H
