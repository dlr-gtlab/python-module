/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_task.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_TASK_H
#define GTPY_TASK_H

#include "gt_task.h"
#include "gt_stringproperty.h"

class GtObjectPathProperty;
class GtpyContextManager;

/**
 * @brief The GtpyTask class
 */
class GtpyTask : public GtTask
{
     Q_OBJECT

public:
    /**
     * @brief GtpyTask
     */
    Q_INVOKABLE GtpyTask();

    /**
     * @brief Adds all available packages to python context and starts
     *  the script evaluation.
     * @return Whether evaluation was successful or not.
     */
    virtual bool runIteration() Q_DECL_OVERRIDE;

    /**
     * @brief Returns current script.
     * @return Current script.
     */
    QString script() const;

    /**
     * @brief Sets new script.
     * @param New script.
     */
    void setScript(QString script);

    /**
     * @brief Returns names of available packages.
     * @return List of names of available packages.
     */
    QStringList packageNames();

    /**
     * @brief Returns the value of the calculator definitions property.
     * @return The value of the calculator definitions property.
     */
    QString calcDefinitions() const;

    /**
     * @brief Sets the calculator definitions property to the given value.
     * @param calcDefinitions Calculator definitions.
     */
    void setCalcDefinitions(QString& calcDefinitions);

protected:
    /**
     * @brief Checks if parent of task is a process data instance. Is this true
     * child will not be accepted.
     * @return Whether child will be accepted or not.
     */
    bool childAccepted(GtObject* /*child*/);

private:
    /**
     * @brief Returns ids of modules.
     * @return List of ids of modules.
     */
    QStringList getModuleIds();

    /// Script.
    GtStringProperty m_script;

    /// Calculator definitions
    GtStringProperty m_calcDefinitions;

    /// Dynamic properties regarding project modules
    QList<GtObjectPathProperty*> m_dynamicPathProps;

    /// Python thread id
    long m_pyThreadId;

private slots:
    void onStateChanged(GtProcessComponent::STATE state);
};

#endif // GTPY_TASK_H
