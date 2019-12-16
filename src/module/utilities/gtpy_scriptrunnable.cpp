/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptrunner.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 16.12.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_scriptrunnable.h"
#include <QDebug>

GtpyScriptRunnable::GtpyScriptRunnable(
    GtpyContextManager::Context contextType) : m_successfulRun(false),
    m_contextType(contextType)
{

}

GtpyScriptRunnable::~GtpyScriptRunnable()
{
    qDebug() << "RUNNABLE DESTROIED " << m_successfulRun;
}

void
GtpyScriptRunnable::run()
{
    m_successfulRun = GtpyContextManager::instance()->evalScript(
                          m_contextType, m_script, true);

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

bool
GtpyScriptRunnable::successful()
{
    return m_successfulRun;
}

