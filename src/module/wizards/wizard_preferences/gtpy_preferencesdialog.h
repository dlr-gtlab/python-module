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

class GtpyPreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    GtpyPreferencesDialog(GtpyEditorPreferences* preferences);

private:
    /**
     * @brief createIcons
     */
    void createIcons();

    void loadSettings();

    ///
    QListWidget* m_contentsWidget;

    ///
    QStackedWidget* m_pagesWidget;

    GtpyEditorPreferences* m_preferences;

private slots:
    void saveChanges();
};

#endif // GTPYWIZARDPREFERENCES_H
