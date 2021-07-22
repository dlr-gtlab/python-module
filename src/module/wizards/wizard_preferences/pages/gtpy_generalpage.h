/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_preferenceseditor.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYPREFERENCESEDITOR_H
#define GTPYPREFERENCESEDITOR_H

#include "gtpy_abstractpreferencespage.h"

class QSpinBox;
class QCheckBox;

/**
 * @brief The GtpyGeneralPage class
 */
class GtpyGeneralPage : public GtpyAbstractPreferencesPage
{
    Q_OBJECT

public:
    /**
     * @brief GtpyGeneralPage
     */
    GtpyGeneralPage();

    /**
     * @brief Passes the user defined preferences to the given pointer pref.
     * @param pref Pointer to the object containing all editor preferences.
     */
    void savePreferences(GtpyEditorPreferences* pref) Q_DECL_OVERRIDE;

private:
    /**
     * @brief Reads the current preferences from the given pointer pref and
     * loads them into the user interface.
     */
    void loadPreferences(GtpyEditorPreferences* pref) Q_DECL_OVERRIDE;

    /// Tab Size
    QSpinBox* m_tabSize;

    /// Replace Tabs By Space
    QCheckBox* m_replaceBySpaces;
};

#endif // GTPYPREFERENCESEDITOR_H
