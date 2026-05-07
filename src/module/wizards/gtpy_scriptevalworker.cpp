/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptevalworker.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 German Aerospace Center (DLR)
 *
 * Created on: 25.03.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_scriptevalworker.h"

#include "gtpy_contextmanager.h"
#include "gtpy_gilscope.h"


GtpyScriptEvalWorker::GtpyScriptEvalWorker(int contextId, QObject* parent) :
    QObject(parent), m_contextId(contextId) { }

void
GtpyScriptEvalWorker::evaluate(const QString& script, bool outputToConsole)
{
    GTPY_GIL_SCOPE

    GtpyContextManager* python = GtpyContextManager::instance();

    {
        QMutexLocker locker(&m_mutex);
        m_threadId = 0;
        m_threadId = python->currentPyThreadId();
    }

    bool success = python->evalScript(m_contextId, script, outputToConsole);

    {
        QMutexLocker locker(&m_mutex);
        m_threadId = 0;
    }

    emit evaluationFinished(success);
}

void
GtpyScriptEvalWorker::interrupt()
{
    GTPY_GIL_SCOPE

    long threadId = 0;

    {
        QMutexLocker locker(&m_mutex);
        threadId = m_threadId;
    }

    if (threadId > 0)
    {
        GtpyContextManager::instance()->interruptPyThread(threadId);
    }
}
