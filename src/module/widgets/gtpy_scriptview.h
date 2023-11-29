/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptview.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.10.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYSCRIPTVIEW_H
#define GTPYSCRIPTVIEW_H

#include "gt_pythonmodule_exports.h"

#include "gt_codeeditor.h"

class GtpyCompleter;

/**
 * @brief The GtpyScriptView class
 */
class GT_PYTHON_EXPORT GtpyScriptView : public GtCodeEditor
{
    Q_OBJECT

public:
    /**
     * @brief GtpyScriptView
     * @param contextId Id of the context in which the scripts written in the
     * editor are evaluated.
     * @param parent Parent of the editor.
     */
    explicit GtpyScriptView(int contextId, QWidget* parent = nullptr);

    /**
     * @brief Handles the tooltips after error messages.
     * @param event received event
     * @return true if the event was recognized and processed
     */
    virtual bool event(QEvent* event) override;

    /**
     * @brief Handles the wheel events.
     * @param event Wheel event
     */
    virtual void wheelEvent(QWheelEvent* event) override;

    /**
     * @brief Returns the written script.
     * @return the written script
     */
    QString script();

    void setScript(const QString& script);

    /**
     * @brief Searchs for the given string searchFor and replaces it
     * with replaceBy.
     * @param searchFor Text to replace.
     * @param replaceBy Text to insert.
     * @param all If it is true, all strings in the document will be replaced.
     * Otherwise it only replaces the first one found.
     */
    bool findAndReplace(const QString& find, const QString& replaceBy,
                        Qt::CaseSensitivity cs = Qt::CaseSensitive);

    void findAndReplaceAll(const QString& find, const QString& replaceBy,
                           Qt::CaseSensitivity cs = Qt::CaseSensitive);

    /**
     * @brief Sets the tab size to the given size.
     * @param size Tab size
     */
    void setIndentSize(int size);

    void replaceTabsBySpaces();

    void selectNextMatch(const QString& text, bool reverse = false,
                         Qt::CaseSensitivity cs = Qt::CaseSensitive);

public slots:
     void setHighlightedText(const QString& text);

protected:
    /**
     * @brief Called when user presses a key.
     * @param e Event which was send by pressing a key.
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief Called when the editor loses the focus. Deletes the highlithing of
     * the current line.
     * @param event Event sent by losing the focus.
     */
    void focusOutEvent(QFocusEvent* event) override;

    /**
     * @brief Called when the editor gets the focused. Highlights the current
     * line.
     * @param event Event sent by getting the focus.
     */
    void focusInEvent(QFocusEvent* event) override;

private slots:
    /**
     * @brief Highlights the current line.
     */
    void lineHighlighting();

    /**
     * @brief Highlights the given line as error line (red).
     * @param codeLine the line number to be highlighted
    */
    void highlightErrorLine(int codeLine);

    /**
     * @brief Receives the error messages.
     * @param message that has been send
     * @param contextId Id of the context in which the error has been triggered.
     */
    void appendErrorMessage(const QString& message, int contextId);

    void resetErrorHighlighting();

    /**
     * @brief Inserts the selected completion into the text editor.
     * @param index of completion
     */
    void insertCompletion();

    void onTextChanged();

private:

    QTextEdit::ExtraSelection m_lineHighlight;

    QTextEdit::ExtraSelection m_errorHighlight;

    QList<QTextEdit::ExtraSelection> m_searchHighlights;

    /// Pointer to cmpleter
    GtpyCompleter* m_cpl;

    /// Error message
    QString m_errorMessage;

    QString m_highlightedText;

    /// Python Context id
    int m_contextId;

    /// Tab size
    int m_indentSize;

    bool m_replaceTabBySpaces;

    void highlightText(const QString& text);

    void replaceSelection(QTextCursor &cursor, const QString& replaceBy) const;

    /**
     * @brief Returns the python code of a function call as string value.
     * @param newVal Value that to be set.
     * @param functionName Name of the called function.
     * @param pyObjName Name of the object that holds the function.
     * @return Python code as string value.
     */
    QString functionCallPyCode(const QString& newVal,
                               const QString& functionName,
                               const QString& pyObjName);

    void commentSelectedLines();

    /**
     * @brief Calls completer to handle completion.
     */
    void handleCompletion();

    /**
     * @brief Checks whether the line in which the cursor is currently located
     *  is commented out.
     * @return True if current line is commented out.
     */
    bool isCurrentLineCommentedOut();

    /**
     * @brief indents a new line
     * @param keyPressEvent
     * @return true if key event was evaluated, false if not
     */
    bool indentNewLine(QKeyEvent* event);
    /**
     * @brief Checks whether the current selection should get indented when
     * pressing e.g. 'tab'
     * @return True if selection should get indented, false if it should get
     * replaced with 'tab'
     */
    bool isSelectionIndentable();

    /**
     * @brief indents selected lines to the right or the left depending on param
     * @param direction, true = right, false = left
     * @return true if lines were indented, false if not
     */
    bool indentSelectedLines(bool direction);

    void setExtraSelections();

    using FindFlags = QTextDocument::FindFlags;
    QTextCursor find(const QString& text, int pos,
                     FindFlags options = FindFlags()) const;

    QTextCursor findAndReplace(const QRegularExpression& expr,
                               const QString& replaceBy, int pos,
                               FindFlags options = FindFlags());

    void findAndReplaceAll(const QRegularExpression& expr,
                           const QString& replaceBy,
                           FindFlags options = FindFlags());

    QTextCursor findNextCursor(const QString& text, const QTextCursor& cursor,
                               FindFlags options = FindFlags()) const;

    void insertFramingCharacters(const QString& character);

signals:
    /**
     * @brief Emitted when eval shortcut (ctrl+E) is pressed.
     */
    void evalShortcutTriggered();

    /**
     * @brief Emitted when find shortcut (ctrl+F) is pressed.
     * @param text Selected text.
     */
    void searchShortcutTriggered(const QString& text);

    /**
     * @brief Emitted when replace shortcut (ctrl+R) is pressed.
     * @param text Selected text.
     */
    void replaceShortcutTriggered(const QString& text);
};

#endif // GTPYSCRIPTVIEW_H
