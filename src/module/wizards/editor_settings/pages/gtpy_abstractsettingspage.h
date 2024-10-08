/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractsettingspage.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 19.07.2021
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYABSTRACTSETTINGSPAGE_H
#define GTPYABSTRACTSETTINGSPAGE_H

#include <QWidget>

#include <gtpy_editorsettings.h>

class QLabel;
class QVBoxLayout;

/**
 * @brief The GtpyAbstractSettingsPage class
 */
class GtpyAbstractSettingsPage : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief GtpyAbstractSettingsPage
     * @param parent
     */
    GtpyAbstractSettingsPage(QWidget* parent = NULL);

    /**
     * @brief ~GtpyAbstractSettingsPage
     */
    virtual ~GtpyAbstractSettingsPage();

    /**
     * @brief In this pure virtual function the user defined settings must
     * be passed to the given pointer pref.
     * @param pref Pointer to the object containing all editor settings.
     */
    virtual void saveSettings(GtpyEditorSettings* pref) = 0;

    /**
     * @brief In this pure virtual function the current settings must be
     * read from the given pointer pref and loaded it into the user interface.
     * @param pref Pointer to the object containing the current editor
     * settings.
     */
    virtual void loadSettings(GtpyEditorSettings* pref) = 0;

    /**
     * @brief Sets the title of the page to the given id.
     * @param id Title of the page
     */
    void setTitle(const QString& id);

    /**
     * @brief Add a stretch factor to the layout of the page.
     * @param val Value of the stretch factor.
     */
    void addStretch(int val = 1);

private:
    /// Label for settings page title
    QLabel* m_title;

    /// Main layout for settings page
    QVBoxLayout* m_layout;
};

#endif // GTPYABSTRACTSETTINGSPAGE_H
