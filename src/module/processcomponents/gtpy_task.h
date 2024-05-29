/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_task.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPY_TASK_H
#define GTPY_TASK_H

#include "gt_task.h"

#include "gtpy_abstractscriptcomponent.h"

/**
 * @brief The GtpyTask class
 */
class GtpyTask : public GtTask, public GtpyAbstractScriptComponent
{
    Q_OBJECT

public:
    /**
     * @brief GtpyTask
     */
    Q_INVOKABLE GtpyTask();

    /**
     * @brief ~GtpyTask
     */
    virtual ~GtpyTask();

    /**
     * @brief Adds all available packages to python context and starts
     *  the script evaluation.
     * @return Whether evaluation was successful or not.
     */
    virtual bool runIteration() override;

protected:
    /**
     * @brief Checks if parent of task is a process data instance. Is this true
     * child will not be accepted.
     * @return Whether child will be accepted or not.
     */
    bool childAccepted(GtObject* /*child*/) override;

private:
    /**
     * @brief Returns the data package identified by the given object path.
     * @param pkgPath Object path
     * @return The data package identified by the given object path
     */
    GtPackage* dataPackage(const GtObjectPath& pkgPath) override;

private slots:
    void onStateChanged(GtProcessComponent::STATE state) const;

signals:
    void deletedFromDatamodel(const QString& uuid);
};

#endif // GTPY_TASK_H
