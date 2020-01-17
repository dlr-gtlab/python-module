/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptrunner.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 16.12.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_scriptrunnable.h"
#include "gtpy_gilscope.h"

GtpyScriptRunnable::GtpyScriptRunnable(
    GtpyContextManager::Context contextType) : m_successfulRun(false),
    m_contextType(contextType)
{

}

GtpyScriptRunnable::~GtpyScriptRunnable()
{

}

void
GtpyScriptRunnable::run()
{
    GTPY_GIL_SCOPE

    GtpyContextManager* python = GtpyContextManager::instance();

    m_mutex.lock();
    m_threadId = python->currentPyThreadId();
    m_mutex.unlock();

    m_successfulRun = python->evalScript(m_contextType,
                                         m_script,
                                         m_outputToConsole);
    emit runnableFinished();
}

QString
GtpyScriptRunnable::script() const
{
    return m_script;
}

void
GtpyScriptRunnable::setScript(const QString& script)
{
    m_script = script;
}

bool GtpyScriptRunnable::outputToConsole() const
{
    return m_outputToConsole;
}

void GtpyScriptRunnable::setOutputToConsole(bool outputToConsole)
{
    m_outputToConsole = outputToConsole;
}

bool
GtpyScriptRunnable::successful()
{
    return m_successfulRun;
}

void
GtpyScriptRunnable::interrupt()
{
    GTPY_GIL_SCOPE

    m_mutex.lock();

    m_successfulRun = false;

    GtpyContextManager::instance()->interruptPyThread(m_threadId);

    m_mutex.unlock();
}

