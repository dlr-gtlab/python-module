/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_interruptrunnable.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 19.12.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_INTERRUPTRUNNABLE_H
#define GTPY_INTERRUPTRUNNABLE_H

#include <QObject>
#include <QRunnable>

class GtpyInterruptRunnable : public QObject,
        public QRunnable
{
    Q_OBJECT
public:
    GtpyInterruptRunnable(long pyThreadId);

    void run() Q_DECL_OVERRIDE;

private:
    long m_pyThreadId;
};

#endif // GTPY_INTERRUPTRUNNABLE_H
