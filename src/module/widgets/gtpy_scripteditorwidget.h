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
class GtpyScriptView;
class GtSearchWidget;

class GT_PYTHON_EXPORT GtpyScriptEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GtpyScriptEditorWidget(int contextId, QWidget* parent = nullptr);

    GtpyScriptView* scriptView() const;

    void setScript(const QString& script);

private:
    GtpyScriptView* m_scriptView;
    QPushButton* m_undoButton;
    QPushButton* m_redoButton;

private slots:

    void setRedoButtonEnabled(bool visible);
    void setUndoButtonEnabled(bool visible);

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

    void onReplace(const QString& find, const QString& replaceBy);
};

#endif // GTPYSCRIPTEDITORWIDGET_H
