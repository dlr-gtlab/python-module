/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptview.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.10.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
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

GtpyScriptView::GtpyScriptView(int contextId, QWidget* parent) :
    GtCodeEditor(parent),
    m_contextId{contextId},
    m_indentSize{4},
    m_replaceTabBySpaces{false}
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

    connect(GtpyContextManager::instance(), SIGNAL(errorCodeLine(int, int)),
            this, SLOT(highlightErrorLine(int, int)));
    connect(GtpyContextManager::instance(), SIGNAL(errorMessage(QString, int)),
            this, SLOT(appendErrorMessage(QString, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this,
            SLOT(resetErrorLine()));
    connect(m_cpl, SIGNAL(activated(QModelIndex)), this,
            SLOT(insertCompletion()));

    disconnect(this, SIGNAL(cursorPositionChanged()), this,
               SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this,
            SLOT(lineHighlighting()));

    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

//    connect(this, SIGNAL(searchShortcutTriggered(QString)), this,
//            SLOT(setHighlightedText(QString)));
}

bool
GtpyScriptView::event(QEvent* event)
{
    if (m_errorLine > -1 && event->type() == QEvent::ToolTip)
    {
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        QTextCursor helpCursor = cursorForPosition(helpEvent->pos());

        if (helpCursor.block().position() == textCursor().block().position())
        {
            QToolTip::setFont(QFontDatabase::systemFont(
                                  QFontDatabase::FixedFont));
            QToolTip::showText(helpEvent->globalPos(),
                               m_errorMessage.isEmpty() ?
                                   "Error" : m_errorMessage.trimmed());
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
                           QFontMetrics(font()).horizontalAdvance(' '));
    }
    else
    {
        GtCodeEditor::wheelEvent(event);
    }
}

QString
GtpyScriptView::script()
{
    resetErrorLine();
    resetErrorMessage();

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
    QTextDocument::FindFlags flags = (cs == Qt::CaseSensitive) ?
                QTextDocument::FindCaseSensitively : QTextDocument::FindFlags();

    /// Move the start position to make sure that the cursor is not already in
    /// the middle of a valid find.
    int startPos = textCursor().position() - find.size();

    /// Find and replace the next match from the cursor position.
    if (startPos > 0)
    {
        if (!findAndReplace(find, replaceBy, startPos, flags).isNull())
        {
            return true;
        }
    }

    /// Find and replace the next match from the beginning of the document.
    return !findAndReplace(find, replaceBy, 0, flags).isNull();
}

void
GtpyScriptView::findAndReplaceAll(const QString& find, const QString& replaceBy,
                                  Qt::CaseSensitivity cs)
{
    QRegExp expr(find);
    expr.setPatternSyntax(QRegExp::FixedString);
    expr.setCaseSensitivity(cs);

    findAndReplaceAll(expr, replaceBy);
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
        int selectionEnd = cursor.selectionEnd();
        cursor.setPosition(cursor.selectionStart());

        /// Calculate the number of spaces that replace the tab
        int tabSize = m_indentSize - cursor.positionInBlock() % m_indentSize;

        cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
        cursor.insertText(QString{tabSize, ' '});

        /// Find next tab
        cursor = document()->find("\t", cursor.position());
    }

    textCursor().endEditBlock();
}

void
GtpyScriptView::keyPressEvent(QKeyEvent* event)
{
    if (m_cpl == nullptr)
    {
        GtCodeEditor::keyPressEvent(event);
        return;
    }

    QToolTip::showText(QPoint(), "");

    if (m_cpl->getPopup()->isVisible())
    {
        switch (event->key())
        {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Tab:
                insertCompletion();
                event->accept();
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

    const bool ctrlModifier = (event->modifiers() & Qt::ControlModifier);
    const bool shiftModifier = (event->modifiers() & Qt::ShiftModifier);

    if (ctrlModifier && shiftModifier)
    {
        switch (event->key())
        {
            case Qt::Key_Apostrophe:
                m_cpl->getPopup()->hide();
                commentSelectedLines();
                return;

            default:
                break;
        }
    }
    else if (ctrlModifier)
    {
        switch (event->key())
        {
            case Qt::Key_E:
                emit evalShortcutTriggered();
                m_cpl->getPopup()->hide();
                return;

            case Qt::Key_F:
                emit searchShortcutTriggered(textCursor().selectedText());
                return;

            case Qt::Key_R:
                emit replaceShortcutTriggered(textCursor().selectedText());
                return;

            case Qt::Key_Space:

                if (!isCurrentLineCommentedOut())
                {
                    handleCompletion();
                }

                return;

            default:
                break;
        }
    }
    else if (shiftModifier)
    {
        switch (event->key())
        {
            case Qt::Key_QuoteDbl:
                m_cpl->getPopup()->hide();
                insertFramingCharacters("\"");
                return;

            default:
                break;
        }
    }

    switch (event->key())
    {
        /* indent new line */
        case Qt::Key_Return:

            if (!indentNewLine(event))
            {
                GtCodeEditor::keyPressEvent(event);
            }

            break;

        /* indent selection with tab to the rigth */
        case Qt::Key_Tab:

            if (!indentSelectedLines(true))
            {
                if (m_replaceTabBySpaces)
                {
                    QTextCursor cursor = this->textCursor();
                    int currentPos = cursor.position();

                    cursor.movePosition(QTextCursor::StartOfLine,
                                        QTextCursor::KeepAnchor);

                    int tabSize = cursor.selectedText().count();

                    tabSize = tabSize % m_indentSize;

                    QString spaces = " ";
                    spaces = spaces.repeated(m_indentSize - tabSize);

                    cursor.setPosition(currentPos);
                    cursor.insertText(spaces);
                }
                else
                {
                    GtCodeEditor::keyPressEvent(event);
                }
            }

            break;

        /* indent selection with backtab to the left */
        case Qt::Key_Backtab:

            if (!indentSelectedLines(false))
            {
                GtCodeEditor::keyPressEvent(event);
            }

            break;

        default:
            GtCodeEditor::keyPressEvent(event);
    }

    QString text = event->text();

    if (!text.isEmpty() && !isCurrentLineCommentedOut()
            && !ctrlModifier)
    {
        handleCompletion();
    }
    else
    {
        m_cpl->getPopup()->hide();
    }
}

void
GtpyScriptView::focusOutEvent(QFocusEvent* event)
{
    removeCurrentLineHighlighting();

    GtCodeEditor::focusOutEvent(event);
}

void
GtpyScriptView::focusInEvent(QFocusEvent* event)
{
    lineHighlighting();

    GtCodeEditor::focusInEvent(event);
}

void
GtpyScriptView::lineHighlighting()
{
    if (!isReadOnly())
    {
        removeCurrentLineHighlighting();

        QList<QTextEdit::ExtraSelection> selectionList = extraSelections();

        QTextEdit::ExtraSelection selection;

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
        auto lineColor = gt::gui::color::code_editor::highlightLine();
        if (gtApp->inDarkMode())
            lineColor.setAlpha(100);
        selection.format.setBackground(lineColor);
#else
        selection.format.setBackground(QColor{Qt::yellow}.lighter(160));
#endif
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        selectionList.prepend(selection);

        setExtraSelections(selectionList);
    }
}

void
GtpyScriptView::highlightErrorLine(int codeLine, int contextId)
{
    if (contextId == m_contextId)
    {
        QList<QTextEdit::ExtraSelection> extraSelections;

        if (!isReadOnly())
        {
            QTextEdit::ExtraSelection selection;

            QColor lineColor = QColor(Qt::red).lighter(160);

            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);

            QTextCursor cr(document()->findBlockByLineNumber(codeLine - 1));
            cr.movePosition(QTextCursor::EndOfLine);

            selection.cursor = cr;
            selection.cursor.clearSelection();
            extraSelections.append(selection);

            setTextCursor(cr);

            m_errorLine = codeLine;
        }

        setExtraSelections(extraSelections);
    }
}

void
GtpyScriptView::appendErrorMessage(const QString& message, int contextId)
{
    if (contextId == m_contextId)
    {
        m_errorMessage = m_errorMessage + message;
    }
}

void
GtpyScriptView::resetErrorLine()
{
    m_errorLine = -1;
}

void
GtpyScriptView::insertCompletion()
{
    if (m_cpl != nullptr)
    {
        QTextCursor tc = textCursor();
        m_cpl->insertCompletion(tc);
        setTextCursor(tc);
        m_cpl->getPopup()->hide();
    }
}

void
GtpyScriptView::onTextChanged()
{
    highlightText(m_highlightedText);
}

void
GtpyScriptView::highlightText(const QString& text)
{
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    auto color = !gtApp->inDarkMode() ?
                QColor{Qt::green}.lighter(160) :
                gt::gui::color::code_editor::highlightLine();
#else
    auto color = QColor{Qt::green}.lighter(160);
#endif

    QList<QTextEdit::ExtraSelection> extraSelections =
            findExtraSelections(text, color);

    setExtraSelections(extraSelections);

    lineHighlighting();
}

bool
GtpyScriptView::moveCursorToNextMatch(const QString& text, QTextCursor& cursor,
                               QTextDocument::FindFlags options) const
{
    cursor = document()->find(text, cursor, options);
    return !cursor.isNull();
}

void
GtpyScriptView::replaceSelection(QTextCursor& cursor,
                                 const QString& replaceBy) const
{
    if (cursor.hasSelection())
    {
        cursor.insertText(replaceBy);
    }
}

void
GtpyScriptView::setHighlightedText(const QString& text)
{
    m_highlightedText = text;
    highlightText(m_highlightedText);
    qDebug() << "setHighlightedText";
    selectNextMatch(m_highlightedText);
}

void
GtpyScriptView::selectNextMatch(const QString& text, bool reverse,
                                Qt::CaseSensitivity cs)
{
    /// translates given options in QTextDocument::FindFlags
    QTextDocument::FindFlags flags = reverse ?
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

QString
GtpyScriptView::functionCallPyCode(const QString& newVal,
                                     const QString& functionName,
                                     const QString& pyObjName)
{
    return pyObjName + "." + functionName + "(" + newVal + ")";
}

void
GtpyScriptView::commentSelectedLines()
{
    QTextCursor cursor = textCursor();

    int startPos = cursor.selectionStart();
    int endPos = cursor.selectionEnd();

    cursor.setPosition(startPos);
    cursor.movePosition(QTextCursor::StartOfLine);

    bool atStartOfLine = cursor.position() == startPos;

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

void GtpyScriptView::resetErrorMessage()
{
    m_errorMessage = QString();
}

bool
GtpyScriptView::isCurrentLineCommentedOut()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
    return cursor.selectedText() == "#";
}

bool
GtpyScriptView::isCurrentLineHighlighted()
{
    QList<QTextEdit::ExtraSelection> selectionList = extraSelections();

    if (selectionList.isEmpty())
    {
        return false;
    }

    QTextEdit::ExtraSelection firstSelection = selectionList.first();
    QVariant fullWidth = firstSelection.format.property(
                             QTextFormat::FullWidthSelection);

    return fullWidth.toBool();
}

void
GtpyScriptView::removeCurrentLineHighlighting()
{
    if (isCurrentLineHighlighted())
    {
        QList<QTextEdit::ExtraSelection> selectionList = extraSelections();

        if (!selectionList.isEmpty())
        {
            selectionList.removeFirst();
            setExtraSelections(selectionList);
        }
    }
}

bool
GtpyScriptView::indentNewLine(QKeyEvent* event)
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);

    QString line = cursor.selectedText();

    /// get leading whitespace characters
    QRegularExpression regExp(QStringLiteral("^\\s+"));
    QRegularExpressionMatch match = regExp.match(line);

    QString newLine = match.captured();
    qDebug() << "newLine: "  << newLine << "!!!!";

    /// check if the last character in the current line is a :
    /// (e.g. def func():)
    regExp.setPattern(QStringLiteral(":\\s*\\Z"));
    match  = regExp.match(line);

    // append tab
    if (match.hasMatch())
    {
        if (m_replaceTabBySpaces)
        {
            QString spaces = " ";
            newLine += spaces.repeated(m_indentSize);
        }
        else
        {
            newLine += QStringLiteral("\t");
        }
    }

    if (newLine.isEmpty())
    {
        return false;
    }

    GtCodeEditor::keyPressEvent(event);

    cursor = textCursor();
    cursor.insertText(newLine);

    return true;
}

bool
GtpyScriptView::isSelectionIndentable()
{
    QTextCursor cursor = this->textCursor();

    if (!cursor.hasSelection())
    {
        return false;
    }

    QString selection = cursor.selectedText();

    QStringList lines = selection.split(QChar::ParagraphSeparator,
                                        QString::SkipEmptyParts);

    int count = lines.count();

    // one line selected -> whole line must be selected to get indented with tab
    if (count == 1)
    {
        cursor.select(QTextCursor::LineUnderCursor);

        QString line = cursor.selectedText();

        int offset = 0;

        for (QChar c : line)
        {
            if (c == ' ' || c == '\t')
            {
                offset++;
            }
            else
            {
                break;
            }
        }

        if (selection.length() == line.length() - offset)
        {
            return true;
        }
    }
    // multiple lines, that are not empty, are selected -> can be indented
    else if (count > 1)
    {
        return true;
    }

    return false;
}

bool
GtpyScriptView::indentSelectedLines(bool direction)
{
    if (!isSelectionIndentable())
    {
        return false;
    }

    QTextCursor cursor = this->textCursor();

    QString selection = cursor.selectedText();

    int count = selection.split(QChar::ParagraphSeparator).count();

    int selectionStart = cursor.selectionStart();

    cursor.setPosition(selectionStart);

    QString tabSpaces = " ";
    tabSpaces = tabSpaces.repeated(m_indentSize);

    cursor.beginEditBlock();

    for (int i = 0; i < count; i++)
    {
        cursor.movePosition(QTextCursor::StartOfLine);

        if (direction) // indent right
        {
            if (m_replaceTabBySpaces)
            {
                cursor.insertText(tabSpaces);
            }
            else
            {
                cursor.insertText(QStringLiteral("\t"));
            }
        }
        else // indent left
        {
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);

            if (m_replaceTabBySpaces)
            {
                if (cursor.selectedText() == tabSpaces)
                {
                    cursor.removeSelectedText();
                }
            }
            else
            {
                if (cursor.selectedText() == QStringLiteral("\t"))
                {
                    cursor.removeSelectedText();
                }
            }
        }

        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.movePosition(QTextCursor::Right);
    }

    cursor.endEditBlock();

    return true;
}

QTextCursor
GtpyScriptView::findAndReplace(const QString& find, const QString& replaceBy,
                               int pos, FindFlags options)
{
    if (find == replaceBy)
    {
        return {};
    }

    QRegExp expr(find);
    expr.setPatternSyntax(QRegExp::FixedString);
    expr.setCaseSensitivity((options & QTextDocument::FindCaseSensitively) ?
                                Qt::CaseSensitive : Qt::CaseInsensitive);

    return findAndReplace(expr, replaceBy, pos, options);
}

QTextCursor
GtpyScriptView::findAndReplace(const QRegExp& expr, const QString& replaceBy,
                               int pos, FindFlags options)
{
    QTextCursor cursor = document()->find(expr, pos, options);

    if (!cursor.isNull())
    {
        int startPos = cursor.selectionStart();
        cursor.insertText(replaceBy);
        cursor.setPosition(startPos);
        cursor.setPosition(startPos + replaceBy.size(), QTextCursor::KeepAnchor);

        setTextCursor(cursor);
    }

    return cursor;
}

void
GtpyScriptView::findAndReplaceAll(const QRegExp& expr, const QString& replaceBy)
{
    textCursor().beginEditBlock();

    /// Find first match in the script and replace it.
    QTextCursor cursor = findAndReplace(expr, replaceBy, 0);

    /// If a match is found and replaced, search for the next match.
    while (!cursor.isNull())
    {
        cursor = findAndReplace(expr, replaceBy, cursor.position());
    }

    textCursor().endEditBlock();
}

QList<QTextEdit::ExtraSelection>
GtpyScriptView::findExtraSelections(const QString& text,
                                    const QColor& color) const
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextCursor cursor = document()->find(text, 0);

    while (!cursor.isNull())
    {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(color);
        selection.cursor = cursor;
        extraSelections.append(selection);

        cursor = document()->find(text, cursor.selectionEnd());
    }

    return extraSelections;
}

QTextCursor
GtpyScriptView::find(const QString& text, int pos, FindFlags options)
{
    if (options & QTextDocument::FindBackward)
    {
        pos = pos > document()->characterCount() ?
                    document()->characterCount() : pos;
    }
    else
    {
        /// ensure that a match is found even if the cursor is positioned in the
        /// middle of a match
        pos = pos - text.length() + 1;
        pos = pos < 0 ? 0 : pos;
    }

    return document()->find(text, pos, options);
}

QTextCursor
GtpyScriptView::findNextCursor(const QString& text, const QTextCursor& cursor,
                               FindFlags options)
{
    int pos = ((cursor.selectedText() == text) &&
                (options & QTextDocument::FindBackward)) ?
                cursor.selectionStart() : cursor.position();

    QTextCursor match = find(text, pos, options);

    if (match.isNull())
    {
        match = (options & QTextDocument::FindBackward) ?
                    find(text, document()->characterCount(), options):
                    find(text, 0, options);
    }

    return match;
}
void
GtpyScriptView::insertFramingCharacters(const QString& character)
{
    QTextCursor cursor = textCursor();
    QString selectedText = cursor.selectedText();

    if (selectedText.isEmpty())
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
        cursor.insertText(selectedText.prepend(character).append(character));
    }
}
