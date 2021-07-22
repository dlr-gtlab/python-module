/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_preferencespage.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYPREFERENCESPAGE_H
#define GTPYPREFERENCESPAGE_H

#include <QWidget>

#include <gtpy_editorpreferences.h>

class QLabel;
class QVBoxLayout;

/**
 * @brief The GtpyAbstractPreferencesPage class
 */
class GtpyAbstractPreferencesPage : public QWidget
{
public:
    /**
     * @brief In this pure virtual function the user defined preferences must
     * be passed to the given pointer pref.
     * @param pref Pointer to the object containing all editor preferences.
     */
    virtual void savePreferences(GtpyEditorPreferences* pref) = 0;

    /**
     * @brief In this pure virtual function the current preferences must be
     * read from the given pointer pref and loaded into the user interface.
     */

    /**
     * @brief In this pure virtual function the current preferences must be
     * read from the given pointer pref and loaded it into the user interface.
     * @param pref Pointer to the object containing the current editor
     * preferences.
     */
    virtual void loadPreferences(GtpyEditorPreferences* pref) = 0;

protected:
    /**
     * @brief GtpyAbstractPreferencesPage
     * @param parent
     */
    GtpyAbstractPreferencesPage(QWidget* parent = NULL);

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
    /// Label for preferences page title
    QLabel* m_title;

    /// Main layout for preferences page
    QVBoxLayout* m_layout;
};

#endif // GTPYPREFERENCESPAGE_H
