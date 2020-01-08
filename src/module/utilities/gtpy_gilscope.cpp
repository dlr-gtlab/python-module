/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_threadsupport.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 17.12.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
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