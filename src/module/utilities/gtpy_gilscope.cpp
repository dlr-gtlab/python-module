/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_threadsupport.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.12.2019
 * Author: Marvin Noethen (AT-TW)
 */

#include "gtpy_gilscope.h"

bool GtpyGilScope::m_enableGILScope = false;

GtpyGilScope::GtpyGilScope() : m_ensured(false)
{
    if (m_enableGILScope)
    {
        m_state = PyGILState_Ensure();
        m_ensured = true;
    }
}

GtpyGilScope::~GtpyGilScope()
{
    release();
}

void
GtpyGilScope::release()
{
    if (m_ensured)
    {
        PyGILState_Release(m_state);
        m_ensured = false;
    }
}

void
GtpyGilScope::setGILScopeEnabled(bool flag)
{
    m_enableGILScope = flag;
}

bool
GtpyGilScope::isGILScopeEnabled()
{
    return m_enableGILScope;
}
