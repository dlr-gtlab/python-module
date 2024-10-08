/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptrunner.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 16.12.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_SCRIPTRUNNER_H
#define GTPY_SCRIPTRUNNER_H

#include <QObject>
#include <QRunnable>
#include <QMutex>

#include "gtpy_contextmanager.h"

/**
 * @brief The GtpyScriptRunnable class
 */
class GtpyScriptRunnable : public QObject,
        public QRunnable
{
    Q_OBJECT

public:
    /**
     * @brief The GtpyScriptRunnable constructor.
     * @param contextId Python context identifier.
     */
    explicit GtpyScriptRunnable(int contextId);

    /**
     * @brief ~GtpyScriptRunnable
     */
    virtual ~GtpyScriptRunnable();

    /**
     * @brief Evaluates the m_script interruptible.
     */
    void run() Q_DECL_OVERRIDE;

    /**
     * @brief Returns the python script.
     * @return The python script.
     */
    QString script() const;

    /**
     * @brief Sets m_script to given script.
     * @param script Python script.
     */
    void setScript(const QString& script);

    /**
     * @brief Returns true when the output of the script evaluation will be send
     * to the console.
     * @return True when the output of the script evaluation will be send
     * to the console.
     */
    bool outputToConsole() const;

    /**
     * @brief Sets the value that determines whether the output will be send
     * to the console.
     * @param outputToConsole True if the output should be send to the console.
     */
    void setOutputToConsole(bool outputToConsole);

    /**
     * @brief Returns whether the script evaluation was successful or not.
     * @return Whether the script evaluation was successful or not.
     */
    bool successful();

private:
    /// Python script
    QString m_script;

    /// Output behaviore
    bool m_outputToConsole;

    /// Evaluation success
    bool m_successfulRun;

    /// Mutex
    QMutex m_mutex;

    /// Current python thread id
    long m_threadId;

    /// Python context identifier
    int m_contextId;

public slots:
    /// Interrupts the python thread with the id m_threadId.
    void interrupt();

signals:
    /// Emitted when the run() function ends.
    void runnableFinished();
};

#endif // GTPY_SCRIPTRUNNER_H
