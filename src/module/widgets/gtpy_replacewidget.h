/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_replacewidget.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 20.10.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYREPLACEWIDGET_H
#define GTPYREPLACEWIDGET_H

#include <QWidget>

class GtpyLineEdit;
class QPushButton;
class QLabel;

class GtpyReplaceWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief GtSearchWidget
     * @param parent
     */
    explicit GtpyReplaceWidget(QWidget* parent = nullptr);

    /**
     * @brief text
     * @return
     */
    QString searchText();

public slots:
    /**
     * @brief Sets text to search line edit.
     * @param text Text to be set to search line edit.
     */
    void setSearchText(const QString& text);

    /**
     * @brief enableSearch
     */
    void enableSearch();

    /**
     * @brief enableReplace
     */
    void enableReplace();

    /**
     * @brief disableSearch
     */
    void disableSearchAndReplace();

private:
    /// Search line
    GtpyLineEdit* m_searchLine;

    /// Search line
    GtpyLineEdit* m_replaceLine;

    /// Search button
    QPushButton* m_searchButton;

    /// Replace button
    QPushButton* m_replaceButton;

    /// Clear search button
    QPushButton* m_clearButton;

    QPushButton* m_backwardButton;

    QPushButton* m_forwardButton;

    void enableSearchComponentes();

    void enableSearchNavigation();
private slots:

    void onSearchTextChanged();

    void onSearchLineFocusOut();

    void onReplaceLineFocusOut();
signals:
    /**
     * @brief searchTextEdited
     */
    void searchTextEdited(const QString&);

    /**
     * @brief searchTextChanged
     */
    void searchTextChanged(const QString&);

    /**
     * @brief searchLineReturnPressed
     */
    void searchLineReturnPressed();

    /**
     * @brief replace
     */
    void replace(const QString&, const QString&);

    /**
     * @brief searchEnabled
     */
    void searchEnabled();

    /**
     * @brief searchAndReplaceDisabled
     */
    void searchAndReplaceDisabled();

    /**
     * @brief searchEnabled
     */
    void replaceEnabled();

    /**
     * @brief searchDisabled
     */
    void replaceDisabled();

    void backwardButtonClicked();

    void forwardButtonClicked();



};

#endif // GTPYREPLACEWIDGET_H
