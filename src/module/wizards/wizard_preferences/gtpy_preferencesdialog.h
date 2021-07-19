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

class QListWidgetItem;
class QListWidget;
class QStackedWidget;

class GtpyPreferencesDialog : public QDialog
{
public:
    GtpyPreferencesDialog();

private:
    /**
     * @brief createIcons
     */
    void createIcons();

    ///
    QListWidget* m_contentsWidget;

    ///
    QStackedWidget* m_pagesWidget;


private slots:
    void saveChanges();
};

#endif // GTPYWIZARDPREFERENCES_H
