/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_interruptrunnable.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 19.12.2019
 * Author: Marvin Noethen (AT-TW)
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
