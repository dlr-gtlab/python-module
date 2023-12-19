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

    void setHighlight(const Highlight& highlight);

    QString selectedText() const;

    bool hasSelection() const;

protected:
    /**
     * @brief Called when user presses a key.
     * @param e Event which was send by pressing a key.
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief Called when the editor loses the focus. Deletes the highlighting of
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

    Highlight m_highlighted;

    /// Error message
    QString m_errorMessage;

    /// Pointer to cmpleter
    GtpyCompleter* m_cpl;

    /// Python Context id
    int m_contextId;

    /// Tab size
    int m_indentSize;

    bool m_replaceTabBySpaces;

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

    QString currentLineIndent(QTextCursor cursor) const;

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

    void highlightText(const Highlight& highlight);

    void insertFramingCharacters(const QString& character);

    QString indentCharacters() const;

    bool insertIndent(QTextCursor cursor);

    bool removeIndent(QTextCursor cursor);
};

#endif // GTPYSCRIPTVIEW_H
