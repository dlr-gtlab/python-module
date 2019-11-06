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
    ~GtpyScriptCalculator();

    /**
     * @brief Main run method of the calculator.
     * @return Whether run process was successful or not.
     */
    bool run() Q_DECL_OVERRIDE;

    /**
     * @brief Returns current script.
     * @return Current script.
     */
    QString script() const;

    /**
     * @brief Sets new script.
     * @param New script.
     */
    void setScript(const QString& script);

    /**
     * @brief Evaluate the given script in python context of the calculator.
     * @param script Script which should be evaluated.
     * @param outputToConsol Whether evaluation output should show in console.
     *  Default value is false.
     * @return True if evaluation was successful.
     */
    bool evalScript(const QString& script, bool outputToConsole = false);

    /**
     * @brief Returns names of available packages.
     * @return List of names of available packages.
     */
    QStringList packageNames();

private:
    /**
     * @brief Returns ids of modules.
     * @return List of ids of modules.
     */
    QStringList getModuleIds();

    /// Script.
    GtStringProperty m_script;

    /// Dynamic properties regarding project modules
    QList<GtObjectPathProperty*> m_dynamicPathProps;

};

#endif // GTPY_SCRIPTCALCULATOR_H
