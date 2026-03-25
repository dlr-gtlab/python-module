/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptevalworker.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 German Aerospace Center (DLR)
 */

#ifndef GTPY_SCRIPTEVALWORKER_H
#define GTPY_SCRIPTEVALWORKER_H

#include <QMutex>
#include <QObject>

/**
 * @brief Worker object that evaluates scripts on a dedicated thread. Its
 * responsibility is limited to executing Python code in an existing context
 * and forwarding interruption requests to the currently active Python thread
 * of that evaluation.
 */
class GtpyScriptEvalWorker : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Creates a worker for the Python context with the given id.
     * @param contextId Identifier of the Python context to evaluate in.
     * @param parent QObject parent.
     */
    explicit GtpyScriptEvalWorker(int contextId, QObject* parent = nullptr);

    /**
     * @brief Requests interruption of the currently running evaluation.
     */
    void interrupt();

public slots:
    /**
     * @brief Evaluates the given script in the worker's Python context.
     * @param script Python code to execute.
     * @param outputToConsole Whether script output should be forwarded to the
     * corresponding console.
     */
    void evaluate(const QString& script, bool outputToConsole);

signals:
    /**
     * @brief Emitted after the evaluation finished.
     * @param success True if the evaluation succeeded, otherwise false.
     */
    void evaluationFinished(bool success);

private:
    /// Python context used by this worker.
    int m_contextId;

    /// Active Python thread id of the current evaluation, or 0 if inactive.
    long m_threadId;

    /// Protects access to m_threadId.
    QMutex m_mutex;
};

#endif // GTPY_SCRIPTEVALWORKER_H
