/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptcalculatorwizardpage.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_SCRIPTCALCULATORWIZARDPAGE_H
#define GTPY_SCRIPTCALCULATORWIZARDPAGE_H

#include "gtpy_scriptcalculator.h"
#include "gtpy_abstractscriptingwizardpage.h"

/**
 * @brief The GtpyScriptCalculatorWizardPage class
 */
class GtpyScriptCalculatorWizardPage : public GtpyAbstractScriptingWizardPage
{
    Q_OBJECT

public:
    /**
     * @brief GtpyScriptCalculatorWizardPage
     */
    Q_INVOKABLE GtpyScriptCalculatorWizardPage();

private:
    /**
     * @brief Sets python script from calculator instance to editor.
     */
    virtual void initialization() Q_DECL_OVERRIDE;

    /**
     * @brief Sets script from editor to calculator instance.
     * @return True if settig script to calculator instance was successful.
     */
    virtual bool validation() Q_DECL_OVERRIDE;

    /**
     * @brief Saves the script into the calculator instance.
     */
    virtual void saveScript() Q_DECL_OVERRIDE;

    /**
     * @brief Returns the uuid of the restored GtpyScriptCalculator.
     * @return uuid of the restored GtpyScriptCalculator
     */
    virtual QString componentUuid() const Q_DECL_OVERRIDE;

    /**
     * @brief Sets the object name of the process component m_calc to the
     * given name.
     * @param name New name of the process component m_calc
     */
    virtual void setComponentName(const QString& name) Q_DECL_OVERRIDE;

    /**
     * @brief Creates a new preferences instance containing the current defined
     * preferences and returns it.
     * @return A new preferences instace containing the current settings.
     */
    virtual GtpyEditorPreferences* createPreferences() Q_DECL_OVERRIDE;

    /**
     * @brief Passes the preferences defined in the given pointer pref to the
     * task.
     * @param pref Current editor preferences.
     */
    virtual void savePreferences(GtpyEditorPreferences* pref) Q_DECL_OVERRIDE;

    /// Calculator
    QPointer<GtpyScriptCalculator> m_calc;
};


#endif // GTPY_SCRIPTCALCULATORWIZARDPAGE_H
