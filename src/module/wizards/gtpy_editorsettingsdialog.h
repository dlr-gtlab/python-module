/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_editorsettingsdialog.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 15.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYEDITORSETTINGSDIALOG_H
#define GTPYEDITORSETTINGSDIALOG_H


#include <QDialog>

class QListWidgetItem;
class QListWidget;
class QStackedWidget;

class GtpyEditorSettingsDialog : public QDialog
{
public:
    GtpyEditorSettingsDialog();

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

#endif // GTPYEDITORSETTINGSDIALOG_H
