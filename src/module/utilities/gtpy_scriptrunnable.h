/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptrunner.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 16.12.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_SCRIPTRUNNER_H
#define GTPY_SCRIPTRUNNER_H

#include <QObject>
#include <QRunnable>

#include "gtpy_contextmanager.h"

class GtpyScriptRunnable : public QObject,
        public QRunnable
{
    Q_OBJECT

public:
    GtpyScriptRunnable(GtpyContextManager::Context contextType);

    virtual ~GtpyScriptRunnable();

    void run() Q_DECL_OVERRIDE;

    QString script() const;

    void setScript(const QString& script);

    bool successful();
private:
    QString m_script;

    bool m_successfulRun;

    GtpyContextManager::Context m_contextType;

signals:
    void runnableFinished();
};

#endif // GTPY_SCRIPTRUNNER_H
