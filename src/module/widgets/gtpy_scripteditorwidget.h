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

#include <memory>

#include <QWidget>

#include "gt_pythonmodule_exports.h"

class QPushButton;
class GtpyScriptView;
class GtSearchWidget;
class GtpyReplaceWidget;

/**
 * @brief The GtpyScriptEditorWidget class
 */
class GT_PYTHON_EXPORT GtpyScriptEditorWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief GtpyScriptEditorWidget
     * @param contextId Context id
     * @param parent Parent widget
     */
    explicit GtpyScriptEditorWidget(int contextId, QWidget* parent = nullptr);

    ~GtpyScriptEditorWidget();

    /**
     * @brief Sets the given script to the script view.
     * @param script Script
     */
    void setScript(const QString& script) const;

    /**
     * @brief Returns the script displayed in the script view.
     * @return The script from the script view.
     */
    QString script() const;

protected:
    /**
     * @brief keyPressEvent
     * @param event Key press event to handle
     */
    void keyPressEvent(QKeyEvent* event) override;

private:
    /// Private implementation
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

    void setSearchText() const;

private slots:

    /**
     * @brief Enables or disables the redo button.
     * @param enable
     */
    void setRedoButtonEnabled(bool enable) const;

    /**
     * @brief Enables or disables the undo button.
     * @param enable
     */
    void setUndoButtonEnabled(bool enable) const;

    /**
     * @brief Instructs the editor to continue the current search backward.
     */
    void onSearchBackward() const;

    /**
     * @brief Instructs the editor to continue the current search.
     */
    void onSearchForward() const;

    /**
     * @brief Finds the given string in the text view and replaces it by
     * replaceBy.
     * @param find Text to find.
     * @param replaceBy Text to insert.
     */
    void onReplace(const QString& find, const QString& replaceBy) const;

    /**
     * @brief Highlights the given text in the text view.
     * @param text Text to highlight.
     */
    void onSearchTextChanged(const QString& text) const;

    /**
     * @brief Enables search widget.
     */
    void onFindShortcut() const;

    /**
     * @brief Enalbes search and replace widget.
     */
    void onReplaceShortcut() const;

signals:
    /**
     * @brief evaluationTriggered
     */
    void evaluationTriggered();
};

#endif // GTPYSCRIPTEDITORWIDGET_H
