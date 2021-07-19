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

class QLabel;
class QVBoxLayout;

#include <QWidget>

class GtpyPreferencesPage : public QWidget
{
public:
    /**
     * @brief saveSettings
     */
    virtual void saveSettings() = 0;

protected:
    GtpyPreferencesPage(QWidget* parent = NULL);

    /**
     * @brief loadSettings
     */
    virtual void loadSettings() = 0;

    /**
     * @brief setTitle
     * @param id
     */
    void setTitle(const QString& id);

    /**
     * @brief addStretch
     * @param val
     */
    void addStretch(int val = 1);

private:
    /// Label for preferences page title
    QLabel* m_title;

    /// Main layout for preferences page
    QVBoxLayout* m_layout;
};

#endif // GTPYPREFERENCESPAGE_H
