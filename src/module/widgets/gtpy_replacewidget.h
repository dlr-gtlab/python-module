/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_replacewidget.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 20.10.2023
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPYREPLACEWIDGET_H
#define GTPYREPLACEWIDGET_H

#include <QWidget>

class GtpyLineEdit;
class QPushButton;

/**
 * @brief The GtpyReplaceWidget class
 */
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
    QString searchText() const;

    /**
     * @brief text
     * @return
     */
    QString replaceText() const;

public slots:

    /**
     * @brief Sets text to the search line edit.
     * @param text Text to be set to the search line edit.
     */
    void setSearchText(const QString& text);

    /**
     * @brief Sets text to the replace line edit.
     * @param text Text to be set to the replace line edit..
     */
    void setReplaceText(const QString& text);

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

protected:

    /**
     * @brief eventFilter
     * @param obj
     * @param event
     * @return
     */
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    /// Search line
    GtpyLineEdit* m_searchLine;

    /// Replace line
    GtpyLineEdit* m_replaceLine;

    /// Search button
    QPushButton* m_searchButton;

    /// Replace button
    QPushButton* m_replaceButton;

    /// Clear search button
    QPushButton* m_clearButton;

    /// Search backward button
    QPushButton* m_backwardButton;

    /// Search forward button
    QPushButton* m_forwardButton;

    /**
     * @brief enableSearchComponentes
     */
    void enableSearchComponentes();

    /**
     * @brief enableSearchNavigation
     */
    void enableSearchNavigation();

private slots:
    /**
     * @brief onSearchTextChanged
     */
    void onSearchTextChanged();

    /**
     * @brief onReplaceClicked
     */
    void onReplaceClicked();
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
     * @brief searchEnabled
     */
    void searchEnabled();

    /**
     * @brief replaceEnabled
     */
    void replaceEnabled();

    /**
     * @brief replace
     */
    void replace(const QString&, const QString&);

    /**
     * @brief searchAndReplaceDisabled
     */
    void searchAndReplaceDisabled();

    /**
     * @brief backwardButtonClicked
     */
    void backwardButtonClicked();

    /**
     * @brief forwardButtonClicked
     */
    void forwardButtonClicked();
};

#endif // GTPYREPLACEWIDGET_H
