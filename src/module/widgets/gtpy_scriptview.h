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

#include "gt_codeeditor.h"

class GtpyCompleter;

/**
 * @brief The GtpyScriptView class
 */
class GtpyScriptView : public GtCodeEditor
{
    Q_OBJECT

public:
    struct Highlight {
        QString text;
        Qt::CaseSensitivity cs{Qt::CaseSensitive};
    };

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
     * @brief Returns the script that is shown in the view.
     * @return The script that is shown in the view.
     */
    QString script();

    /**
     * @brief Sets the given script so that it is shown in the view.
     * @param script The script to be set.
     */
    void setScript(const QString& script);

    /**
     * @brief Searches for the given string and replaces it by replaceBy.
     * @param find Text to search for.
     * @param replaceBy Text that replaces the match.
     * @param cs Specifies whether the search should be CaseSensitive or
     * CaseInsensitive.
     */
    bool findAndReplace(const QString& find, const QString& replaceBy,
                        Qt::CaseSensitivity cs = Qt::CaseSensitive);

    /**
     * @brief Searches for all occurrences of the given string find and
     * replaces them with replaceBy.
     * @param find Text to search for.
     * @param replaceBy Text that replaces the matches.
     * @param cs Specifies whether the search should be CaseSensitive or
     * CaseInsensitive.
     */
    void findAndReplaceAll(const QString& find, const QString& replaceBy,
                           Qt::CaseSensitivity cs = Qt::CaseSensitive);

    /**
     * @brief Sets the indent size to the given size.
     * @param size Indent size.
     */
    void setIndentSize(int size);

    /**
     * @brief Repalces all tab characters by spaces.
     */
    void replaceTabsBySpaces();

    /**
     * @brief Selects the next string in the script view that matches the
     * given text.
     * @param text Text to find.
     * @param backward If True, the search is carried out backwards.
     * @param cs Specifies whether the search should be CaseSensitive or
     * CaseInsensitive.
     */
    void selectNextMatch(const QString& text, bool backward = false,
                         Qt::CaseSensitivity cs = Qt::CaseSensitive);

    /**
     * @brief Set text to highlight.
     * @param highlight Text to highlight.
     */
    void setHighlight(const Highlight& highlight);

    /**
     * @brief Returns the currently selected text.
     * @return The currently selected text.
     */
    QString selectedText() const;

    /**
     * @brief Returns ture if text is selected.
     * @return Ture if text is selected.
     */
    bool hasSelection() const;

protected:
    /**
     * @brief Called when user presses a key.
     * @param e Event which was send by pressing a key.
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief Called when the editor loses the focus. Deletes the highlighting
     * of the current line.
     * @param event Event sent by losing the focus.
     */
    void focusOutEvent(QFocusEvent* event) override;

    /**
     * @brief Called when the editor gets the focused. Highlights the current
     * line.
     * @param event Event sent by getting the focus.
     */
    void focusInEvent(QFocusEvent* event) override;

private:
    /// Line highlight
    QTextEdit::ExtraSelection m_lineHighlight;

    /// Error highlight
    QTextEdit::ExtraSelection m_errorHighlight;

    /// Search highlights
    QList<QTextEdit::ExtraSelection> m_searchHighlights;

    /// Currently highlighted text
    Highlight m_highlighted;

    /// Error message
    QString m_errorMessage;

    /// Pointer to cmpleter
    GtpyCompleter* m_cpl;

    /// Python Context id
    int m_contextId;

    /// Tab size
    int m_indentSize;

    /// Whether tabs are replaced by spaces
    bool m_replaceTabBySpaces;

    /**
     * @brief Comments out the lines the cursor seletes.
     * @param cursor
     */
    void commentSelectedLines(QTextCursor cursor);

    /**
     * @brief Calls completer to handle completion.
     */
    void handleCompletion();

    /**
     * @brief Checks whether the line in which the cursor is currently located
     *  is commented out.
     * @return True if current line is commented out.
     */
    bool isCurrentLineCommentedOut(QTextCursor cursor) const;

    /**
     * @brief Returns the indent characters for the line in which the cursor
     * is located.
     * @param cursor Text cursor.
     * @return The indent characters for the line in which the cursor
     * is located.
     */
    QString currentLineIndent(QTextCursor cursor) const;

    /**
     * @brief Sets the extra selections consisting of line highlighting,
     * error highlighting and search highlighting.
     */
    void setExtraSelections();

    using FindFlags = QTextDocument::FindFlags;

    /**
     * @brief Finds the next cursor from the given position that matches the
     * given text and returns it.
     * @param text Text to search for.
     * @param pos Position from which the search is started.
     * @param options Find flags.
     * @return The next cursor that matches the given text.
     */
    QTextCursor find(const QString& text, int pos,
                     FindFlags options = FindFlags()) const;

    /**
     * @brief Searches for the next string that matches the regular expression
     * and replaces it by replaceBy.
     * @param expr Regular expression for the search.
     * @param replaceBy Text that replaces the match.
     * @param pos Position from which the search is started.
     * @param options Find flags.
     * @return The cursor after replacing the string. It is null if no match
     * was found.
     */
    QTextCursor findAndReplace(const QRegularExpression& expr,
                               const QString& replaceBy, int pos,
                               FindFlags options = FindFlags());

    /**
     * @brief Searches for all strings that match the regular expression and
     * replaces them with replaceBy.
     * @param expr Regular expression for the search.
     * @param replaceBy Text that replaces the match.
     * @param options Find flags.
     */
    void findAndReplaceAll(const QRegularExpression& expr,
                           const QString& replaceBy,
                           FindFlags options = FindFlags());

    /**
     * @brief Searches for the next occurrences of the given text, starting
     * from the position of the specified cursor. It returns the cursor that
     * selects the text that was searched for.
     * @param text Text to search for.
     * @param cursor Cursor to start searching from.
     * @param options Find flags.
     * @return The next cursor that selects the text that was searched for.
     */
    QTextCursor findNextCursor(const QString& text, const QTextCursor& cursor,
                               FindFlags options = FindFlags()) const;

    /**
     * @brief Highlights the specified text.
     * @param highlight Text to be highlighted
     */
    void highlightText(const Highlight& highlight);

    /**
     * @brief Inserts the given character twice and places the cursor in the
     * middle of the characters (e.g. for inserting quotation marks).
     * @param character Character to insert.
     */
    void insertFramingCharacters(const QString& character);

    /**
     * @brief Returns the indentation characters that correspond to the
     * current settings. It is a user-defined number of spaces or a
     * tab character.
     * @return The indentation characters.
     */
    QString indentCharacters() const;

    /**
     * @brief Inserts an indent in the line where the cursor is located and in
     * all lines that the cursor selects. It returns true if the indent
     * was successful.
     * @param cursor The text cursor that selects the lines to be indented.
     */
    void insertIndent(QTextCursor cursor);

    /**
     * @brief Removes an indent from the line in which the cursor is located
     * and from all lines that the cursor selects. It returns true if the
     * removal of the indent was successful.
     * @param cursor The text cursor that selects the lines the indent should
     * be removed from.
     * @return True if the removal of the indent was successful.
     */
    bool removeIndent(QTextCursor cursor);

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

    /**
     * @brief Resets the error message and the error highlighting.
     */
    void resetErrorHighlighting();

    /**
     * @brief Inserts the selected completion into the text editor.
     * @param index of completion
     */
    void insertCompletion();

    /**
     * @brief onTextChanged
     */
    void onTextChanged();
};

#endif // GTPYSCRIPTVIEW_H
