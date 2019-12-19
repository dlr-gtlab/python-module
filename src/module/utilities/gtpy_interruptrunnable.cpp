/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_interruptrunnable.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 19.12.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_interruptrunnable.h"
#include "gtpy_gilscope.h"

GtpyInterruptRunnable::GtpyInterruptRunnable(long pyThreadId) :
    m_pyThreadId(pyThreadId)
{

}

void
GtpyInterruptRunnable::run()
{
    GTPY_GIL_SCOPE

    PyThreadState_SetAsyncExc(m_pyThreadId, PyExc_KeyboardInterrupt);
}
