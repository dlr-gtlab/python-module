/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptview.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 19.10.2023
 * Author: Marvin Noethen (AT-TW)
 */

#include <QAbstractItemView>
#include <QTextBlock>
#include <QToolTip>
#include <QFontMetrics>
#include <QRegularExpression>
#include <QMimeData>
#include <QScrollBar>
#include <QTextDocumentFragment>

#include "gt_application.h"

#include "gtpy_completer.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_colors.h"
#endif

#include "gtpy_scriptview.h"

namespace {

int
lineFromMessage(const QString& message)
{
    static QRegularExpression re{R"((?<="<string>", line )\d+)"};
    QRegularExpressionMatch match = re.match(message);

    if (!match.hasMatch())
    {
        return -1;
    }

    return match.captured().toInt() - 1;
}

bool
iterateSelectedLines(QTextCursor& c, std::function<void(QTextCursor&)> callback)
{
    int lineCount = c.selectedText().count(QChar::ParagraphSeparator);

    c.beginEditBlock();

    c.setPosition(c.selectionStart());

    /// iterate over the selected lines
    for (int i = 0; i <= lineCount; ++i)
    {
        c.movePosition(QTextCursor::StartOfLine);

        callback(c);

        if(!c.movePosition(QTextCursor::Down))
        {
            break;
        }
    }

    c.endEditBlock();

    return true;
}

}

GtpyScriptView::GtpyScriptView(int contextId, QWidget* parent) :
    GtCodeEditor(parent),
    m_cpl{nullptr},
    m_contextId{contextId},
    m_indentSize{4},
    m_replaceTabBySpaces{true}
{
    QFont sysFont;
#ifdef Q_OS_LINUX
    sysFont.setFamily("Monospace");
#elif defined(Q_OS_MAC)
    sysFont.setFamily("Menlo");
#else
    sysFont.setFamily("Consolas");
#endif
    setFont(sysFont);

    m_cpl = new GtpyCompleter(m_contextId, this);

    setMouseTracking(true);
    setAcceptDrops(false);
    setIndentSize(m_indentSize);

    QPalette p = palette();
    const QColor cHighlight = p.color(QPalette::Active, QPalette::Highlight);
    const QColor cHighlightText = p.color(QPalette::Active,
                                          QPalette::HighlightedText);

    p.setColor(QPalette::Inactive, QPalette::Highlight, cHighlight);
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, cHighlightText);
    setPalette(p);

    /// current line highlighting
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    auto lineColor = gt::gui::color::code_editor::highlightLine();
    if (gtApp->inDarkMode())
        lineColor.setAlpha(100);
    m_lineHighlight.format.setBackground(lineColor);
#else
    m_lineHighlight.format.setBackground(QColor{Qt::yellow}.lighter(160));
#endif
    m_lineHighlight.format.setProperty(QTextFormat::FullWidthSelection, true);

    /// error line highlighting
    m_errorHighlight.format.setBackground(QColor{Qt::red}.lighter(160));
    m_errorHighlight.format.setProperty(QTextFormat::FullWidthSelection, true);

    connect(GtpyContextManager::instance(), SIGNAL(errorMessage(QString, int)),
            this, SLOT(appendErrorMessage(QString, int)));
    connect(m_cpl, SIGNAL(activated(QModelIndex)), this,
            SLOT(insertCompletion()));

    disconnect(this, SIGNAL(cursorPositionChanged()), this,
               SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this,
            SLOT(lineHighlighting()));

    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

bool
GtpyScriptView::event(QEvent* event)
{
    if (event->type() == QEvent::ToolTip)
    {
        if (!m_errorHighlight.cursor.isNull())
        {
            QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);

            if (cursorForPosition(helpEvent->pos()).block().position() ==
                    m_errorHighlight.cursor.block().position())
            {
                QToolTip::setFont(QFontDatabase::systemFont(
                                      QFontDatabase::FixedFont));
                QToolTip::showText(helpEvent->globalPos(),
                                   m_errorMessage.trimmed());
            }
        }
    }

    return QPlainTextEdit::event(event);
}

void
GtpyScriptView::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() == Qt::ControlModifier && !isReadOnly())
    {
        if (event->angleDelta().y() > 0)
        {
            zoomIn(2);
        }
        else
        {
            zoomOut(2);
        }

        setTabStopDistance(m_indentSize *
                           QFontMetrics{font()}.horizontalAdvance(' '));
        return;
    }

    GtCodeEditor::wheelEvent(event);
}

QString
GtpyScriptView::script()
{
    return toPlainText();
}

void
GtpyScriptView::setScript(const QString& script)
{
    setPlainText(script);
}

bool
GtpyScriptView::findAndReplace(const QString& find, const QString& replaceBy,
                               Qt::CaseSensitivity cs)
{
    if (find.isEmpty())
    {
        return false;
    }
    else if (find == replaceBy)
    {
        return true;
    }

    QTextDocument::FindFlags flags = (cs == Qt::CaseSensitive) ?
                QTextDocument::FindCaseSensitively : QTextDocument::FindFlags();

    /// Move the start position to make sure that the cursor is not already in
    /// the middle of a valid find.
    int startPos = textCursor().position() - find.size();

    QRegularExpression expr{find};

    /// Find and replace the next match from the cursor position.
    if (startPos > 0)
    {
        if (!findAndReplace(expr, replaceBy, startPos, flags).isNull())
        {
            return true;
        }
    }

    /// Find and replace the next match from the beginning of the document.
    return !findAndReplace(expr, replaceBy, 0, flags).isNull();
}

void
GtpyScriptView::findAndReplaceAll(const QString& find, const QString& replaceBy,
                                  Qt::CaseSensitivity cs)
{
    if (find == replaceBy)
    {
        return;
    }

    QTextDocument::FindFlags flags = (cs == Qt::CaseSensitive) ?
                QTextDocument::FindCaseSensitively : QTextDocument::FindFlags();
    findAndReplaceAll(QRegularExpression{find}, replaceBy, flags);
}

void
GtpyScriptView::setIndentSize(int size)
{
    m_indentSize = size;
    setTabStopDistance(m_indentSize * QFontMetrics(font()).horizontalAdvance(' '));
}

void
GtpyScriptView::replaceTabsBySpaces()
{
    textCursor().beginEditBlock();

    QTextCursor cursor = document()->find("\t", 0);

    while (!cursor.isNull())
    {
        /// Calculate the number of spaces that replace the tab
        int tabSize = m_indentSize -
                (cursor.positionInBlock() - 1) % m_indentSize;

        cursor.insertText(QString{tabSize, ' '});

        /// Find next tab
        cursor = document()->find("\t", cursor.position());
    }

    textCursor().endEditBlock();
}

void
GtpyScriptView::keyPressEvent(QKeyEvent* event)
{
    QToolTip::hideText();

    if (m_cpl->getPopup()->isVisible())
    {
        switch (event->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Tab:
            insertCompletion();
            return;

        case Qt::Key_Right:
        case Qt::Key_Left:
            GtCodeEditor::keyPressEvent(event);
            handleCompletion();
            return;

        default:
            break;
        }
    }

    if (event->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier))
    {
        switch (event->key())
        {
        case Qt::Key_Apostrophe:
            commentSelectedLines(textCursor());
            return;

        default:
            break;
        }
    }
    else if (event->modifiers() == Qt::ControlModifier)
    {
        switch (event->key())
        {
        case Qt::Key_Space:
            if (!isCurrentLineCommentedOut(textCursor()))
            {
                handleCompletion();
            }

            return;

        default:
            break;
        }
    }
    else if (event->modifiers() == Qt::ShiftModifier)
    {
        switch (event->key())
        {
        case Qt::Key_QuoteDbl:
            insertFramingCharacters("\"");
            return;

        case Qt::Key_Apostrophe:
            insertFramingCharacters("\'");
            return;

        /// remove indentation from selected text
        case Qt::Key_Backtab:
            removeIndent(textCursor());
            return;

        default:
            break;
        }
    }
    else
    {
        switch (event->key())
        {
        /// indent new line
        case Qt::Key_Return:
            GtCodeEditor::keyPressEvent(event);
            insertPlainText(currentLineIndent(textCursor()));
            return;

        /// indent selected text
        case Qt::Key_Tab:
            insertIndent(textCursor());
            return;

        default:
            break;
        }
    }

    GtCodeEditor::keyPressEvent(event);

    if (!event->text().isEmpty() && !isCurrentLineCommentedOut(textCursor()))
    {
        handleCompletion();
    }
}

void
GtpyScriptView::focusOutEvent(QFocusEvent* event)
{
    m_lineHighlight.cursor = {};
    setExtraSelections();

    GtCodeEditor::focusOutEvent(event);
}

void
GtpyScriptView::focusInEvent(QFocusEvent* event)
{
    lineHighlighting();

    GtCodeEditor::focusInEvent(event);
}

void
GtpyScriptView::selectNextMatch(const QString& text, bool backward,
                                Qt::CaseSensitivity cs)
{
    /// translates given options in QTextDocument::FindFlags
    QTextDocument::FindFlags flags = backward ?
                QTextDocument::FindBackward : QTextDocument::FindFlags();
    flags = (cs == Qt::CaseSensitive) ?
                (flags | QTextDocument::FindCaseSensitively) : flags;

    /// find next match
    QTextCursor cursor = textCursor();
    QTextCursor nextCursor = findNextCursor(text, cursor, flags);

    if (!nextCursor.isNull())
    {
        setTextCursor(nextCursor);
    }
    else
    {
        cursor.setPosition(cursor.selectionStart());
        setTextCursor(cursor);
    }
}

void
GtpyScriptView::setHighlight(const Highlight& highlight)
{
    m_highlighted = highlight;
    highlightText(m_highlighted);
    selectNextMatch(m_highlighted.text, false, m_highlighted.cs);
}

QString
GtpyScriptView::selectedText() const
{
    return textCursor().selectedText();
}

bool
GtpyScriptView::hasSelection() const
{
    return textCursor().hasSelection();
}

void
GtpyScriptView::commentSelectedLines(QTextCursor cursor)
{
    cursor.beginEditBlock();

    int startPos = cursor.selectionStart();
    int endPos = cursor.selectionEnd();

    cursor.setPosition(startPos);
    cursor.movePosition(QTextCursor::StartOfLine);

    bool atStartOfLine = (cursor.position() == startPos);

    cursor.setPosition(endPos, QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    QString selectedText =  cursor.selection().toPlainText().prepend("\n");

    if (selectedText.contains("\n#"))
    {
        endPos -= selectedText.count("\n#");
        startPos = atStartOfLine || !selectedText.startsWith("\n#") ?
                    startPos : startPos - 1;
        selectedText.replace("\n#", "\n");
    }
    else
    {
        endPos += selectedText.count("\n");
        startPos = atStartOfLine ? startPos : startPos + 1;
        selectedText.replace("\n", "\n#");
    }

    selectedText.remove(0, 1);

    cursor.insertText(selectedText);
    cursor.setPosition(startPos);
    cursor.setPosition(endPos, QTextCursor::KeepAnchor);

    setTextCursor(cursor);

    cursor.endEditBlock();
}

void GtpyScriptView::handleCompletion()
{
    if (m_cpl != nullptr)
    {
        QTextCursor cursor = textCursor();
        QRect rect = cursorRect(cursor);
        m_cpl->handleTabCompletion(cursor, rect);
    }
}

bool
GtpyScriptView::isCurrentLineCommentedOut(QTextCursor cursor) const
{
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    return cursor.selectedText().contains("#");
}

QString
GtpyScriptView::currentLineIndent(QTextCursor cursor) const
{
    cursor.movePosition(QTextCursor::Up);
    cursor.select(QTextCursor::LineUnderCursor);

    /// get leading whitespace characters
    static QRegularExpression indentRegExp{R"(^\s+)"};
    QString indent = indentRegExp.match(cursor.selectedText()).captured();

    /// check if the last character in the current line is a colon
    /// (e.g. def func():)
    static QRegularExpression colonRegExp{R"(:\s*\Z)"};

    if (colonRegExp.match(cursor.selectedText()).hasMatch())
    {
        indent.append(indentCharacters());
    }

    return indent;
}

void
GtpyScriptView::setExtraSelections()
{
    QList<QTextEdit::ExtraSelection> es{m_lineHighlight, m_errorHighlight};
    es.append(m_searchHighlights);
    GtCodeEditor::setExtraSelections(es);
}

QTextCursor
GtpyScriptView::find(const QString& text, int pos, FindFlags options) const
{
    /// ensure that a match is found even if the cursor is positioned in the
    /// middle of a match
    pos = (options & QTextDocument::FindBackward) ?
                pos : pos - text.length() + 1;

    if (pos < 0)
    {
        pos = 0;
    }
    else if (pos >= document()->characterCount())
    {
        pos = document()->characterCount() - 1;
    }

    return document()->find(text, pos, options);
}

QTextCursor
GtpyScriptView::findAndReplace(const QRegularExpression& expr,
                               const QString& replaceBy, int pos,
                               FindFlags options)
{
    QTextCursor cursor = document()->find(expr, pos, options);

    if (!cursor.isNull())
    {
        cursor.insertText(replaceBy);
        setTextCursor(cursor);
    }

    return cursor;
}

void
GtpyScriptView::findAndReplaceAll(const QRegularExpression& expr,
                                  const QString& replaceBy, FindFlags options)
{
    textCursor().beginEditBlock();

    /// Find first match in the script and replace it.
    QTextCursor cursor = findAndReplace(expr, replaceBy, 0);

    /// If a match is found and replaced, search for the next match.
    while (!cursor.isNull())
    {
        cursor = findAndReplace(expr, replaceBy, cursor.position(), options);
    }

    textCursor().endEditBlock();
}

QTextCursor
GtpyScriptView::findNextCursor(const QString& text, const QTextCursor& cursor,
                               FindFlags options) const
{
    QString selectedText{cursor.selectedText()};
    bool findBackward = (options & QTextDocument::FindBackward);

    /// determine the correct starting position
    int pos = ((selectedText == text && findBackward) ||
               ((selectedText != text) && selectedText.startsWith(text))) ?
                cursor.selectionStart() : cursor.position();

    QTextCursor match = find(text, pos, options);

    if (match.isNull())
    {
        match = findBackward ?
                    find(text, document()->characterCount(), options):
                    find(text, 0, options);
    }

    return match;
}

void
GtpyScriptView::highlightText(const Highlight& highlight)
{
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    auto color = !gtApp->inDarkMode() ?
                QColor{Qt::green}.lighter(160) :
                gt::gui::color::code_editor::highlightLine();
#else
    auto color = QColor{Qt::green}.lighter(160);
#endif

    m_searchHighlights.clear();

    FindFlags flags = (highlight.cs == Qt::CaseSensitive) ?
                QTextDocument::FindCaseSensitively : QTextDocument::FindFlags();

    QTextCursor cursor = find(highlight.text, 0, flags);

    while (!cursor.isNull())
    {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(color);
        selection.cursor = cursor;
        m_searchHighlights.append(selection);

        cursor = find(highlight.text, cursor.selectionEnd(), flags);
    }

    setExtraSelections();
}

void
GtpyScriptView::insertFramingCharacters(const QString& character)
{
    QTextCursor cursor = textCursor();

    if (!cursor.hasSelection())
    {
        insertPlainText(character);
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);

        if ((cursor.selectedText().count(character) % 2) != 0)
        {
            insertPlainText(character);
            moveCursor(QTextCursor::PreviousCharacter);
        }
    }
    else
    {
        cursor.insertText(cursor.selectedText()
                          .prepend(character)
                          .append(character));
    }
}

QString
GtpyScriptView::indentCharacters() const
{
    return m_replaceTabBySpaces ? QString{" "}.repeated(m_indentSize) : "\t";
}

void
GtpyScriptView::insertIndent(QTextCursor cursor)
{
    QString selectedText{cursor.selectedText()};
    QString indent{indentCharacters()};

    /// check if cursor has no selection
    if (selectedText.isEmpty())
    {
        cursor.insertText(indent);
    }
    /// check if multiple lines are selected
    else if (selectedText.count(QChar::ParagraphSeparator) > 0)
    {
        iterateSelectedLines(cursor, [&indent](QTextCursor& c){
            c.insertText(indent);
        });
    }
    else
    {
        QTextCursor cursorCopy{cursor};
        cursorCopy.select(QTextCursor::LineUnderCursor);

        /// check if not the entire text of the line is selected
        if (selectedText != cursorCopy.selectedText())
        {
            cursor.insertText(indent);
            return;
        }

        cursorCopy.movePosition(QTextCursor::StartOfLine);
        cursorCopy.insertText(indent);
        cursorCopy.select(QTextCursor::LineUnderCursor);
        setTextCursor(cursorCopy);
    }
}

bool
GtpyScriptView::removeIndent(QTextCursor cursor)
{
    QString selectedText{cursor.selectedText()};

    /// check if only one line is selected
    if (selectedText.count(QChar::ParagraphSeparator) == 0)
    {
        cursor.select(QTextCursor::LineUnderCursor);

        /// check if the entire text of the line is selected
        if (selectedText != cursor.selectedText())
        {
            return false;
        }
    }

    QRegularExpression re{};
    re.setPattern(QString{"^(\\t| {1,%1}\\t| {%2})"}
                  .arg(QString::number(m_indentSize - 1),
                       QString::number(m_indentSize)));

    auto removeLineIndet = [&re](QTextCursor& c){

        c.movePosition(QTextCursor::StartOfLine);
        c.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

        QRegularExpressionMatch match = re.match(c.selectedText());

        if (match.hasMatch())
        {
            c.movePosition(QTextCursor::StartOfLine);
            c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                                match.capturedLength());
            c.removeSelectedText();
        }
    };

    return iterateSelectedLines(cursor, removeLineIndet);
}

void
GtpyScriptView::lineHighlighting()
{
    if (!isReadOnly())
    {
        m_lineHighlight.cursor = textCursor();
        m_lineHighlight.cursor.clearSelection();

        resetErrorHighlighting();

        setExtraSelections();
    }
}

void
GtpyScriptView::highlightErrorLine(int codeLine)
{
    if (!isReadOnly())
    {
        m_errorHighlight.cursor = QTextCursor{
                document()->findBlockByLineNumber(codeLine)};
        m_lineHighlight.cursor = {};

        setExtraSelections();
    }
}

void
GtpyScriptView::appendErrorMessage(const QString& message, int contextId)
{
    if (contextId == m_contextId)
    {
        m_errorMessage += message;

        int line = lineFromMessage(message);

        if (line > -1)
        {
            highlightErrorLine(line);
        }
    }
}

void
GtpyScriptView::resetErrorHighlighting()
{
    m_errorHighlight.cursor = {};
    m_errorMessage = "";
    setExtraSelections();
}

void
GtpyScriptView::insertCompletion()
{
    if (m_cpl != nullptr)
    {
        QTextCursor cursor = textCursor();
        m_cpl->insertCompletion(cursor);
        setTextCursor(cursor);
        m_cpl->getPopup()->hide();
    }
}

void
GtpyScriptView::onTextChanged()
{
    highlightText(m_highlighted);
}
