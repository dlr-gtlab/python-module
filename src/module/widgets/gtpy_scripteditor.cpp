/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scripteditor.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QAbstractItemView>
#include <QTextBlock>
#include <QToolTip>
#include <QFontMetrics>
#include <QDebug>

#include "gtpy_completer.h"

#include "gtpy_scripteditor.h"

GtpyScriptEditor::GtpyScriptEditor(GtpyContextManager::Context type,
                                   QWidget* parent) : GtCodeEditor(parent)
{
    const QFont sysFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    setFont(sysFont);

    m_cpl = new GtpyCompleter(type, this);

    m_contextType = type;

    setMouseTracking(true);

    const int tabStop = 4;
    QFontMetrics metrics(font());
    setTabStopWidth(tabStop * metrics.width(' '));

    QPalette p = palette();
    const QColor cHighlight = p.color(QPalette::Active, QPalette::Highlight);
    const QColor cHighlightText = p.color(QPalette::Active,
                                          QPalette::HighlightedText);
    p.setColor(QPalette::Inactive, QPalette::Highlight, cHighlight);
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, cHighlightText);
    setPalette(p);

    setAcceptDrops(false);

    GtpyContextManager* python = GtpyContextManager::instance();

    connect(python, SIGNAL(errorCodeLine(int,
            GtpyContextManager::Context)), this,
          SLOT(highlightErrorLine(int,GtpyContextManager::Context)));
    connect(python, SIGNAL(errorMessage(QString,
                  GtpyContextManager::Context)), this,
         SLOT(appendErrorMessage(QString,GtpyContextManager::Context)));
    connect(this, SIGNAL(cursorPositionChanged()), this,
         SLOT(resetErrorLine()));
    connect(m_cpl, SIGNAL(activated(QModelIndex)), this,
            SLOT(insertCompletion()));

    disconnect(this, SIGNAL(cursorPositionChanged()), this,
               SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this,
            SLOT(lineHighlighting()));
}

bool GtpyScriptEditor::event(QEvent* event)
{
    if (event->type() == QEvent::ToolTip)
    {
        if (m_errorLine == -1)
        {
            return QPlainTextEdit::event(event);
        }

        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        QTextCursor helpCursor = cursorForPosition(helpEvent->pos());

        int helpCursorPos =  helpCursor.block().position();
        int currentCursorPos = textCursor().block().position();

        if (helpCursorPos == currentCursorPos)
        {
            QString toolTipStr;

            if (m_errorMessage.isEmpty())
            {
                toolTipStr = "Error";
            }
            else
            {
                int index = m_errorMessage.lastIndexOf("\n");
                int size = m_errorMessage.size();

                if (size == index + 1)
                {
                    m_errorMessage = m_errorMessage.left(index);
                }

                toolTipStr = m_errorMessage;
            }

            QFont font = QToolTip::font();

            font.setStyleHint(QFont::Monospace);
            font.setFixedPitch(true);

            const QFont sysFont = QFontDatabase::systemFont(
                                      QFontDatabase::FixedFont);
            QToolTip::setFont(sysFont);

            QToolTip::showText(helpEvent->globalPos(), toolTipStr);
        }
    }

    return QPlainTextEdit::event(event);
}

QString
GtpyScriptEditor::script()
{
    resetErrorLine();
    resetErrorMessage();

    return toPlainText();
}

void
GtpyScriptEditor::insertToCurrentCursorPos(const QString& text)
{
    QTextCursor cursor = textCursor();

    cursor.clearSelection();

    cursor.insertText(text);
}

void
GtpyScriptEditor::replaceIntoBlock(const QString& header,
                                   const QString& caption,
                                   const QString& newVal,
                                   const QString& functionName,
                                   const QString& pyObjName)
{
    if (header.isEmpty() || caption.isEmpty() || newVal.isEmpty() ||
            functionName.isEmpty() || pyObjName.isEmpty())
    {
        return;
    }

    QTextCursor startCursor = textCursor();

    startCursor.clearSelection();

    startCursor.movePosition(QTextCursor::Start);

    startCursor = document()->find(header, startCursor);

    if (startCursor.position() > -1)
    {
        QString selectedText;

        bool moveDown = startCursor.movePosition(QTextCursor::Down);

        if (moveDown)
        {
            moveDown = startCursor.movePosition(QTextCursor::Down);

            while (moveDown)
            {
                startCursor.movePosition(QTextCursor::StartOfLine);

                startCursor.movePosition(QTextCursor::EndOfLine,
                                         QTextCursor::KeepAnchor);

                selectedText = startCursor.selectedText();

                if (selectedText.contains(pyObjName + "." + functionName))
                {
                    selectedText.trimmed();

                    int startVal = selectedText.indexOf("(") + 1;
                    int endVal = selectedText.size() - 1;

                    selectedText.replace(startVal, endVal - startVal, newVal);

                    startCursor.insertText(selectedText);
                    return;
                }

                if (selectedText.contains(caption))
                {
                    break;
                }

                moveDown = startCursor.movePosition(QTextCursor::Down);
            }

            startCursor.clearSelection();

            if (moveDown)
            {
                startCursor.movePosition(QTextCursor::Up);
            }

            startCursor.movePosition(QTextCursor::EndOfLine);

            startCursor.insertText("\n");

            startCursor.insertText(functionCallPyCode(newVal, functionName,
                                               pyObjName));
        }
    }
}

void
GtpyScriptEditor::replaceBlockHeaders(const QString& oldHeader,
                                      const QString& newHeader,
                                      const QString& oldCaption,
                                      const QString& newCaption)
{
    if (oldHeader.isEmpty() || oldCaption.isEmpty())
    {
        return;
    }

    QTextCursor startCursor = textCursor();

    startCursor.clearSelection();

    startCursor.movePosition(QTextCursor::Start);

    startCursor = document()->find(oldHeader, startCursor);

    startCursor.movePosition(QTextCursor::StartOfLine);

    startCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    if (startCursor.hasSelection())
    {
        startCursor.insertText(newHeader);
    }

    QTextCursor endCursor = document()->find(oldCaption, startCursor);

    endCursor.movePosition(QTextCursor::StartOfLine);

    endCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    if (endCursor.hasSelection())
    {
        endCursor.insertText(newCaption);
    }
}

void
GtpyScriptEditor::searchAndReplace(const QRegExp& searchRegExp, const
                                   QString& replaceBy, bool all)
{
    if (searchRegExp.isEmpty() || !searchRegExp.isValid())
    {
        return;
    }

    QTextCursor cursor = textCursor();

    cursor.clearSelection();

    if (all)
    {
        cursor.movePosition(QTextCursor::Start);

        cursor = document()->find(searchRegExp, cursor);

        while (!cursor.isNull())
        {
            if (cursor.hasSelection())
            {
                cursor.insertText(replaceBy);
            }

            cursor = document()->find(searchRegExp, cursor);
        }
    }
    else
    {
        cursor = document()->find(searchRegExp, cursor);

        if (cursor.hasSelection())
        {
            cursor.insertText(replaceBy);
        }
    }
}

void
GtpyScriptEditor::searchAndReplace(const QString& searchFor,
                                   const QString& replaceBy,
                                   bool all)
{
    if (searchFor.isEmpty() || replaceBy.isEmpty())
    {
        return;
    }

    QTextCursor cursor = textCursor();

    cursor.clearSelection();

    if (all)
    {
        cursor.movePosition(QTextCursor::Start);

        cursor = document()->find(searchFor, cursor);

        while (!cursor.isNull())
        {
            if (cursor.hasSelection())
            {
                cursor.insertText(replaceBy);
            }

            cursor = document()->find(searchFor, cursor);
        }
    }
    else
    {
        cursor = document()->find(searchFor, cursor);

        if (cursor.hasSelection())
        {
            cursor.insertText(replaceBy);
        }
    }
}

void
GtpyScriptEditor::searchHighlighting(const QString& searchText,
                                     bool moveToNextFound)
{
    m_lastSearch = searchText;

    if (!searchText.isEmpty() && !isReadOnly())
    {
        QList<QTextEdit::ExtraSelection> extraSelections;

        QTextCursor highlightingCursor = textCursor();
        highlightingCursor.movePosition(QTextCursor::Start);
        highlightingCursor = document()->find(searchText, highlightingCursor);

        QList<QTextCursor> cursorList;

        while (!highlightingCursor.isNull())
        {
            cursorList.append(highlightingCursor);

            highlightingCursor = document()->find(searchText,
                                                  highlightingCursor);
        }

        for (int i = 0; i < cursorList.size(); i++)
        {
            QTextEdit::ExtraSelection selection;

            QColor color = QColor(Qt::green).lighter(160);;

            selection.format.setBackground(color);
            selection.cursor = cursorList.at(i);
            extraSelections.append(selection);
        }

        setExtraSelections(extraSelections);

        if (moveToNextFound)
        {
            highlightingCursor = textCursor();
            highlightingCursor.movePosition(QTextCursor::StartOfLine);
            highlightingCursor = document()->find(searchText,
                                                  highlightingCursor);

            if (highlightingCursor.isNull())
            {
                highlightingCursor = document()->find(searchText,
                                              highlightingCursor,
                                              QTextDocument::FindBackward);
            }

            if (!highlightingCursor.isNull())
            {
                setTextCursor(highlightingCursor);
            }
        }
    }
    else
    {
        removeSearchHighlighting();
    }

    lineHighlighting();
}

void
GtpyScriptEditor::removeSearchHighlighting()
{
    m_SearchActivated = false;

    QList<QTextEdit::ExtraSelection> selectionList = extraSelections();

    if (!selectionList.isEmpty())
    {
        if (isCurrentLineHighlighted())
        {
            QTextEdit::ExtraSelection firstSelection = selectionList.first();
            selectionList.clear();
            selectionList << firstSelection;
        }
        else
        {
            selectionList.clear();
        }
    }

    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
    setExtraSelections(selectionList);
}

void
GtpyScriptEditor::textSearchingActivated()
{
    m_SearchActivated = true;
}

void
GtpyScriptEditor::moveCursorToNextFound()
{
    QTextCursor cursor = textCursor();
    cursor.clearSelection();

    cursor = document()->find(m_lastSearch,
                              cursor);

    if (cursor.isNull())
    {
        cursor = document()->find(m_lastSearch);
    }

    if (!cursor.isNull())
    {
        setTextCursor(cursor);
    }
}

void
GtpyScriptEditor::moveCursorToLastFound()
{
    QTextCursor cursor = textCursor();

    int startPos = cursor.selectionStart();
    cursor.clearSelection();
    cursor.setPosition(startPos);

    cursor = document()->find(m_lastSearch, cursor,
                              QTextDocument::FindBackward);

    if (cursor.isNull())
    {
        cursor = textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor = document()->find(m_lastSearch, cursor,
                                  QTextDocument::FindBackward);
    }

    if (!cursor.isNull())
    {
        setTextCursor(cursor);
    }
}

void
GtpyScriptEditor::keyPressEvent(QKeyEvent* event)
{
    if (m_cpl == Q_NULLPTR)
    {
        GtCodeEditor::keyPressEvent(event);
    }



    const bool ctrlModifier = (event->modifiers() & Qt::ControlModifier);
    const bool shiftModifier = (event->modifiers() & Qt::ShiftModifier);

    QToolTip::showText(QPoint(), "");

    if (m_cpl->getPopup()->isVisible())
    {
        switch (event->key())
        {
            case Qt::Key_Enter:
            case Qt::Key_Return:

                insertCompletion();
                event->accept();
                return;

            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Left:
                GtCodeEditor::keyPressEvent(event);
                handleCompletion();
                return;

            case Qt::Key_Right:
                GtCodeEditor::keyPressEvent(event);
                handleCompletion();
                return;

            case Qt::Key_Backtab:
                event->ignore();
                return;

            case Qt::Key_Down:

            default:
                break;
        }
    }

    if (ctrlModifier && shiftModifier)
    {
        switch (event->key())
        {
            case Qt::Key_Apostrophe:
                m_cpl->getPopup()->hide();

                if (textCursor().selectedText().isEmpty())
                {
                    commentLine(!isCurrentLineCommentedOut());
                }
                else
                {
                    commentOutBlock();
                }

                return;

            default:
                break;
        }
    }
    else if (ctrlModifier)
    {
        QString text;
        QTextCursor cursor;

        switch (event->key())
        {
            case Qt::Key_E:
                emit evalShortcutTriggered();
                m_cpl->getPopup()->hide();
                return;

            case Qt::Key_F:
                cursor = textCursor();
                text = cursor.selectedText();
                emit searchShortcutTriggered(text);
                searchHighlighting(text);
                setTextCursor(cursor);
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
        QTextCursor cursor;

        switch (event->key())
        {
            case Qt::Key_QuoteDbl:

                m_cpl->getPopup()->hide();

                cursor = textCursor();

                if (cursor.selectedText().isEmpty())
                {
                    GtCodeEditor::keyPressEvent(event);

                    QTextCursor temp = cursor;

                    cursor.movePosition(QTextCursor::StartOfLine,
                                        QTextCursor::KeepAnchor);

                    QString selectedText = cursor.selectedText();

                    int qouteCount = selectedText.count("\"");

                    if ((qouteCount % 2) != 0)
                    {
                        insertPlainText("\"");
                        temp.movePosition(QTextCursor::PreviousCharacter);
                        setTextCursor(temp);
                    }
                }
                else
                {
                    QString selectedText = cursor.selectedText();

                    selectedText = "\"" + selectedText + "\"";

                    cursor.insertText(selectedText);
                }

                return;

            default:
                break;

        }
    }

    switch (event->key())
    {
        case Qt::Key_Return:
        {
            QTextCursor cursor;
            cursor = textCursor();
            cursor.movePosition(QTextCursor::StartOfLine,
                                QTextCursor::KeepAnchor);

            QString text;
            text = cursor.selectedText();

            int asciiTab = 9;

            QChar tab = static_cast<char>(asciiTab);

            int i = 0;

            int textSize = text.size();

            while (i < textSize)
            {
                if (text.startsWith(tab))
                {
                    text = text.mid(1);
                    i++;
                }
                else
                {
                    break;
                }
            }

            GtCodeEditor::keyPressEvent(event);

            if (m_SearchActivated)
            {
                searchHighlighting(m_lastSearch, false);
            }

            cursor = textCursor();

            for (int j = 0; j < i; j++)
            {
                cursor.insertText(tab);
            }

            return;
        }
        default:
            break;
    }

    GtCodeEditor::keyPressEvent(event);

    if (m_SearchActivated)
    {
        searchHighlighting(m_lastSearch, false);
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
GtpyScriptEditor::focusOutEvent(QFocusEvent* event)
{
    removeCurrentLineHighlighting();

    GtCodeEditor::focusOutEvent(event);
}

void
GtpyScriptEditor::focusInEvent(QFocusEvent* event)
{
    lineHighlighting();

    GtCodeEditor::focusInEvent(event);
}

void
GtpyScriptEditor::lineHighlighting()
{
    if (!isReadOnly())
    {
        removeCurrentLineHighlighting();

        QList<QTextEdit::ExtraSelection> selectionList = extraSelections();

        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        selectionList.prepend(selection);

        setExtraSelections(selectionList);
    }
}

void
GtpyScriptEditor::highlightErrorLine(int codeLine,
                                  const GtpyContextManager::Context& type)
{
    if (type == m_contextType)
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

void GtpyScriptEditor::appendErrorMessage(const QString& message,
                                   const GtpyContextManager::Context& type)
{
    if (type == m_contextType)
    {
        m_errorMessage = m_errorMessage + message;
    }
}

void GtpyScriptEditor::resetErrorLine()
{
    m_errorLine = -1;
}

void GtpyScriptEditor::insertCompletion()
{
    if (m_cpl == Q_NULLPTR)
    {
        return;
    }

    QTextCursor tc = textCursor();

    m_cpl->insertCompletion(tc);

    setTextCursor(tc);

    m_cpl->getPopup()->hide();
}

QString
GtpyScriptEditor::functionCallPyCode(const QString& newVal,
                              const QString& functionName,
                              const QString& pyObjName)
{
    return pyObjName + "." + functionName + "(" + newVal + ")";
}

void GtpyScriptEditor::commentLine(bool commentOut)
{
    QTextCursor cursor = textCursor();

    if (cursor.block().length() == 1)
    {
        return;
    }

    QTextCursor temp = cursor;

    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.clearSelection();

    if (!commentOut)
    {
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);

        QString selection = cursor.selectedText();

        if (selection == "#")
        {
            cursor.removeSelectedText();
            cursor.clearSelection();
        }
    }
    else
    {
        setTextCursor(cursor);
        insertPlainText("#");
    }

    setTextCursor(temp);
}

void GtpyScriptEditor::commentOutBlock()
{
    QTextCursor cursor = textCursor();
    QTextCursor temp = cursor;

    QString selectedText = cursor.selectedText();

    if (selectedText.isEmpty())
    {
        return;
    }

    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();

    cursor.movePosition(QTextCursor::End);

    cursor.setPosition(start);
    cursor.movePosition(QTextCursor::StartOfLine);

    bool outCommenting = true;
    bool lastLine = false;

    while (end >= cursor.position() && outCommenting && !lastLine)
    {
        if (isCurrentLineCommentedOut())
        {
            outCommenting = false;
        }

        lastLine = !cursor.movePosition(QTextCursor::Down);
        setTextCursor(cursor);
    }

    setTextCursor(temp);
    cursor = textCursor();
    cursor.setPosition(start);
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.clearSelection();

    lastLine = false;

    while (end >= cursor.position() && !lastLine)
    {
        if (cursor.block().length() > 1)
        {
            setTextCursor(cursor);
            commentLine(outCommenting);

            if (!outCommenting)
            {
                end--;
            }
            else
            {
                end++;
            }
        }

        lastLine = !cursor.movePosition(QTextCursor::Down);
        cursor.movePosition(QTextCursor::StartOfLine);
    }

    setTextCursor(temp);
}

void GtpyScriptEditor::handleCompletion()
{
    QTextCursor c = this->textCursor();
    QRect cr = cursorRect(c);

    m_cpl->handleTabCompletion(c, cr);
}

void GtpyScriptEditor::resetErrorMessage()
{
    m_errorMessage = QString();
}

bool GtpyScriptEditor::isCurrentLineCommentedOut()
{
    QTextCursor cursor = textCursor();

    if (cursor.block().length() == 1)
    {
        return false;
    }

    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);

    QString selection = cursor.selectedText();

    if (selection == "#")
    {
        return true;
    }

    return false;
}

bool
GtpyScriptEditor::isCurrentLineHighlighted()
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
GtpyScriptEditor::removeCurrentLineHighlighting()
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
