/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_threadsupport.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 17.12.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_THREADSUPPORT_H
#define GTPY_THREADSUPPORT_H

#include "PythonQtPythonInclude.h"

#include <QObject>

#define GTPY_GIL_SCOPE GtpyGilScope internal_gilscope;

/**
 * @brief The GtpyGilScope class
 */
class GtpyGilScope : public QObject
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
