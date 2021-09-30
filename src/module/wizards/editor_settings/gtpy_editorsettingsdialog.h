/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_editorsettingsdialog.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYEDITORSETTINGSDIALOG_H
#define GTPYEDITORSETTINGSDIALOG_H

#include <QDialog>

#include "gtpy_editorsettings.h"

class QListWidgetItem;
class QListWidget;
class QStackedWidget;

/**
 * @brief The GtpyEditorSettingsDialog class
 */
class GtpyEditorSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief GtpyEditorSettingsDialog
     * @param settings
     */
    explicit GtpyEditorSettingsDialog(GtpyEditorSettings* settings);

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

    /// Settings
    GtpyEditorSettings* m_settings;

private slots:
    /**
     * @brief Saves all currently defined settings.
     */
    void saveChanges();
};

#endif // GTPYEDITORSETTINGSDIALOG_H