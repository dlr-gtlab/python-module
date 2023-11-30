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
class GtpyReplaceWidget;

class GT_PYTHON_EXPORT GtpyScriptEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GtpyScriptEditorWidget(int contextId, QWidget* parent = nullptr);

    GtpyScriptView* scriptView() const;

    void setScript(const QString& script) const;

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    GtpyScriptView* m_scriptView;
    QPushButton* m_undoButton;
    QPushButton* m_redoButton;
    GtpyReplaceWidget* m_replaceWidget;

    void setSearchText() const;
private slots:

    void setRedoButtonEnabled(bool visible) const;
    void setUndoButtonEnabled(bool visible) const;

    /**
     * @brief Instructs the editor to continue the current search backward.
     */
    void onSearchBackward() const;

    /**
     * @brief Instructs the editor to continue the current search.
     */
    void onSearchForward() const;

    void onReplace(const QString& find, const QString& replaceBy) const;

    void onSearchTextChanged(const QString& text) const;

    void onFindShortcut() const;

    void onReplaceShortcut() const;

signals:
    void evaluationTriggered();
};

#endif // GTPYSCRIPTEDITORWIDGET_H
