/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptcalculatorwizardpage.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
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
    virtual void initialization() override;

    /**
     * @brief Sets script from editor to calculator instance.
     * @return True if settig script to calculator instance was successful.
     */
    virtual bool validation() override;

    /**
     * @brief Returns the uuid of the restored GtpyScriptCalculator.
     * @return uuid of the restored GtpyScriptCalculator
     */
    virtual QString componentUuid() const override;

    /**
     * @brief Sets the object name of the process component m_calc to the
     * given name.
     * @param name New name of the process component m_calc
     */
    virtual void setComponentName(const QString& name) override;

    /**
     * @brief Creates a new settings instance containing the current defined
     * settings and returns it.
     * @return A new settings instace containing the current settings.
     */
    virtual GtpyEditorSettings* createSettings() override;

    /**
     * @brief Passes the settings defined in the given pointer pref to the
     * task.
     * @param pref Current editor settings.
     */
    virtual void saveSettings(GtpyEditorSettings* pref) override;

    /// Calculator
    QPointer<GtpyScriptCalculator> m_calc;
};


#endif // GTPY_SCRIPTCALCULATORWIZARDPAGE_H
