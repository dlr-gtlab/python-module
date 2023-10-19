/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scripteditorwidget.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.10.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYSCRIPTEDITORWIDGET_H
#define GTPYSCRIPTEDITORWIDGET_H

#include <QWidget>

#include "gt_pythonmodule_exports.h"

class QPushButton;
class GtpyScriptEditor;
class GtSearchWidget;

class GT_PYTHON_EXPORT GtpyScriptEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GtpyScriptEditorWidget(int contextId, QWidget* parent = nullptr);

    GtpyScriptEditor* editor() const;

private:
    GtpyScriptEditor* m_editor;
    QPushButton* m_undoButton;
    QPushButton* m_redoButton;
    QPushButton* m_backwardButton;
    QPushButton* m_forwardButton;
    GtSearchWidget* m_searchWidget;

    void enableSearchNavigation(bool enable);
private slots:

    void setRedoButtonVisible(bool visible);
    void setUndoButtonVisible(bool visible);

    /**
     * @brief Sets the search forward button and the search backward button
     * visible. Notifies the editor that the search is enabled.
     */
    void onSearchEnabled();

    /**
     * @brief Sets the search forward button and the search backward button
     * invisible. Notifies the editor that the search is disabled.
     */
    void onSearchDisabled();

    /**
     * @brief Instructs the editor to continue the current search backward.
     */
    void onSearchBackward();

    /**
     * @brief Instructs the editor to continue the current search.
     */
    void onSearchForward();

    /**
     * @brief Enables and disables forward and backward buttons.
     */
    void onSearchTextEdit();

    /**
     * @brief Sets text to search widget.
     * @param text Text to be set on the search widget.
     */
    void setSearchedText(const QString& text);
};

#endif // GTPYSCRIPTEDITORWIDGET_H
