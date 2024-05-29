/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_interruptrunnable.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 19.12.2019
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPY_INTERRUPTRUNNABLE_H
#define GTPY_INTERRUPTRUNNABLE_H

#include <QObject>
#include <QRunnable>

/**
 * @brief The GtpyInterruptRunnable class
 */
class GtpyInterruptRunnable : public QObject,
        public QRunnable
{
    Q_OBJECT

public:
    /**
     * @brief The GtpyInterruptRunnable constructor.
     * @param pyThreadId Id of the python thread to be interrupted.
     */
    explicit GtpyInterruptRunnable(long pyThreadId);

    /**
     * @brief Throws an KeyboardInterrupt to the python thread with the id
     *  m_pyThreadId.
     */
    void run() Q_DECL_OVERRIDE;

private:
    /// Thread id
    long m_pyThreadId;
};

#endif // GTPY_INTERRUPTRUNNABLE_H
