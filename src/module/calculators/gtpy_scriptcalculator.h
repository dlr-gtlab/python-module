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
#include "gt_stringproperty.h"
#include "gt_boolproperty.h"
#include "gt_intproperty.h"

class GtpyContextManager;

/**
 * @brief The GtpyScriptCalculator class
 */
class GtpyScriptCalculator : public GtCalculator
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
     * @brief Returns whether tabs are replaced by spaces.
     * @return True if tabs are replaced by spaces.
     */
    bool replaceTabBySpaces() const;

    /**
     * @brief Sets whether tabs are replaced by spaces.
     * @param replaceTabBySpaces If this parameter is true tabs will be
     * replaced by spaces.
     */
    void setReplaceTabBySpaces(bool replaceTabBySpaces);

    /**
     * @brief Returns the tab size.
     * @return Tab size
     */
    int tabSize() const;

    /**
     * @brief Sets the tab size to the given tabSize.
     * @param tabSize Tab size value.
     */
    void setTabSize(int tabSize);

private:
    /**
     * @brief If connection is true, it connects to the stateChanged
     * signal of its root task. Else it disconnects from the root task.
     * @param connection Whether the connection is to be established or
     * disconnected.
     */
    void connectWithRootTask(bool connection);

    /// Script.
    GtStringProperty m_script;


    /// Replace Tab By Spaces.
    GtBoolProperty m_replaceTabBySpaces;

    /// Tab size.
    GtIntProperty m_tabSize;

    /// Dynamic properties regarding project modules
    QList<GtObjectPathProperty*> m_dynamicPathProps;

    /// Python thread id
    long m_pyThreadId;

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
    void onTaskStateChanged(GtProcessComponent::STATE state);

signals:
    void deletedFromDatamodel(const QString& uuid);
};

#endif // GTPY_SCRIPTCALCULATOR_H
