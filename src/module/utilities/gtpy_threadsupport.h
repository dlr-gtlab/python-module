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

#include <PythonQtPythonInclude.h>

#define GTPY_GIL_SCOPE GtpyGilScope internal_gilscope;

class GtpyGilScope
{
public:
    GtpyGilScope();

    ~GtpyGilScope();

    void release();

    static void setGILScopeEnabled(bool flag);

    static bool isGILScopeEnabled();

private:
    PyGILState_STATE m_state;
    bool m_ensured;

    static bool m_enableGILScope;
};

#endif // GTPY_THREADSUPPORT_H
