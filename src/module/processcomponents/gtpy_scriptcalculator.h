/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptcalculator.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_SCRIPTCALCULATOR_H
#define GTPY_SCRIPTCALCULATOR_H

#include "gt_calculator.h"

#include "gtpy_abstractscriptcomponent.h"

/**
 * @brief The GtpyScriptCalculator class
 */
class GtpyScriptCalculator : public GtCalculator, public GtpyAbstractScriptComponent
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     */
    Q_INVOKABLE GtpyScriptCalculator();

    /**
     * @brief Destructor.
     */
    virtual ~GtpyScriptCalculator();

    /**
     * @brief Main run method of the calculator.
     * @return Whether run process was successful or not.
     */
    bool run() override;

private:
    /**
     * @brief If connection is true, it connects to the stateChanged
     * signal of its root task. Else it disconnects from the root task.
     * @param connection Whether the connection is to be established or
     * disconnected.
     */
    void connectWithRootTask(bool connection);

    /**
     * @brief Returns the data package identified by the given object path.
     * @param pkgPath Object path
     * @return The data package identified by the given object path
     */
    GtPackage* dataPackage(const GtObjectPath& pkgPath) override;

private slots:
    /**
     * @brief When the state changes to RUNNING, this function establishes a
     * connection to the root task.
     * @param state State of this calculator.
     */
    void onStateChanged(GtProcessComponent::STATE state);

    /**
     * @brief When the state of the root task changes to TERMINATION_REQUESTED,
     * the evaluation of the script will be interrupted.
     * @param state State of the root task.
     */
    void onTaskStateChanged(GtProcessComponent::STATE state) const;

signals:
    void deletedFromDatamodel(const QString& uuid);
};

#endif // GTPY_SCRIPTCALCULATOR_H
