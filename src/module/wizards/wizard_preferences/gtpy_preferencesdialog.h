/* GTlab - Gas Turbine laboratory
 * Source File: gtpywizardpreferences.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYWIZARDPREFERENCES_H
#define GTPYWIZARDPREFERENCES_H

#include <QDialog>

#include "gtpy_editorpreferences.h"

class QListWidgetItem;
class QListWidget;
class QStackedWidget;

/**
 * @brief The GtpyPreferencesDialog class
 */
class GtpyPreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief GtpyPreferencesDialog
     * @param preferences
     */
    GtpyPreferencesDialog(GtpyEditorPreferences* preferences);

private:
    /**
     * @brief Creates all the page icons
     */
    void createIcons();

    /**
     * @brief Loads the current setting.
     */
    void loadSettings();

    /// Contents Widget
    QListWidget* m_contentsWidget;

    /// Pages Widget
    QStackedWidget* m_pagesWidget;

    /// Preferences
    GtpyEditorPreferences* m_preferences;

private slots:
    /**
     * @brief Saves all currently defined preferences.
     */
    void saveChanges();
};

#endif // GTPYWIZARDPREFERENCES_H
