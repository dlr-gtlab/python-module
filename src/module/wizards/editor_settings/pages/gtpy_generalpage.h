/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_generalpage.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYGENERALPAGE_H
#define GTPYGENERALPAGE_H

#include "gtpy_abstractsettingspage.h"

class QSpinBox;
class QCheckBox;

/**
 * @brief The GtpyGeneralPage class
 */
class GtpyGeneralPage : public GtpyAbstractSettingsPage
{
    Q_OBJECT

public:
    /**
     * @brief GtpyGeneralPage
     */
    explicit GtpyGeneralPage(QWidget* parent = NULL);

    /**
     * @brief Passes the user defined settings to the given pointer pref.
     * @param pref Pointer to the object containing all editor settings.
     */
    virtual void saveSettings(GtpyEditorSettings* pref) override;

    /**
     * @brief Reads the current settings from the given pointer pref and
     * loads them into the user interface.
     */
    virtual void loadSettings(GtpyEditorSettings* pref) override;

private:
    /// Tab Size
    QSpinBox* m_tabSize;

    /// Replace Tabs By Space
    QCheckBox* m_replaceBySpaces;
};

#endif // GTPYGENERALPAGE_H
