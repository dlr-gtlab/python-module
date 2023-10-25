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
#include <QRegularExpression>
#include <QMimeData>
#include <QScrollBar>

#include "gt_objectmemento.h"
#include "gt_objectfactory.h"
#include "gt_application.h"

#include "gtpy_completer.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_colors.h"
#endif

#include "gtpy_scriptview.h"

GtpyScriptView::GtpyScriptView(int contextId, QWidget* parent) :
    GtCodeEditor(parent), m_searchActivated(false), m_tabSize(4),
    m_replaceTabBySpaces(false)
{
    //const QFont sysFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    QFont sysFont;
#ifdef Q_OS_LINUX
    sysFont.setFamily("Monospace");
#elif defined(Q_OS_MAC)
    sysFont.setFamily("Menlo");
#else
    sysFont.setFamily("Consolas");
#endif
    setFont(sysFont);

    m_cpl = new GtpyCompleter(contextId, this);

    m_contextId = contextId;

    setMouseTracking(true);

    setTabSize(4);

    QPalette p = palette();
    const QColor cHighlight = p.color(QPalette::Active, QPalette::Highlight);
    const QColor cHighlightText = p.color(QPalette::Active,
                                          QPalette::HighlightedText);

    p.setColor(QPalette::Inactive, QPalette::Highlight, cHighlight);
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, cHighlightText);
    setPalette(p);

    setAcceptDrops(false);

    GtpyContextManager* python = GtpyContextManager::instance();

    connect(python, SIGNAL(errorCodeLine(int, int)), this,
            SLOT(highlightErrorLine(int, int)));
    connect(python, SIGNAL(errorMessage(QString, int)), this,
            SLOT(appendErrorMessage(QString, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this,
            SLOT(resetErrorLine()));
    connect(m_cpl, SIGNAL(activated(QModelIndex)), this,
            SLOT(insertCompletion()));

    disconnect(this, SIGNAL(cursorPositionChanged()), this,
               SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this,
            SLOT(lineHighlighting()));
}

bool
GtpyScriptView::event(QEvent* event)
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

void
GtpyScriptView::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() == Qt::ControlModifier && !isReadOnly())
    {
        if (event->delta() > 0)
        {
            zoomIn(2);
        }
        else
        {
            zoomOut(2);
        }

        setTabSize(m_tabSize);
    }
    else
    {
        QPlainTextEdit::wheelEvent(event);
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
GtpyScriptView::insertToCurrentCursorPos(const QString& text)
{
    QTextCursor cursor = textCursor();

    cursor.clearSelection();

    cursor.insertText(text);
}

void
GtpyScriptView::replaceIntoBlock(const QString& header,
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
                    //selectedText.trimmed();

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
GtpyScriptView::replaceBlockHeaders(const QString& oldHeader,
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
GtpyScriptView::searchAndReplace(const QRegExp& searchRegExp, const
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
GtpyScriptView::searchAndReplace(const QString& searchFor,
                                   const QString& replaceBy,
                                   bool all)
{
    if (searchFor.isEmpty() || replaceBy.isEmpty())
    {
        return;
    }

    QTextCursor cursor = textCursor();
    cursor.setPosition(cursor.selectionStart());
    cursor.clearSelection();
    qDebug() << "after pos: " << cursor.position();
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
GtpyScriptView::acceptCalculatorDrops(bool accept)
{
    setAcceptDrops(accept);
}

int
GtpyScriptView::cursorPosition()
{
    return textCursor().position();
}

void
GtpyScriptView::setCursorPosition(int pos)
{
    QTextCursor cursor = textCursor();

    cursor.setPosition(pos);

    setTextCursor(cursor);

    centerCursor();
}

int
GtpyScriptView::verticalSliderPos()
{
    return verticalScrollBar()->sliderPosition();
}

void
GtpyScriptView::setVerticalSliderPos(int pos)
{
    verticalScrollBar()->setSliderPosition(pos);
}

void GtpyScriptView::setTabSize(int size)
{
    m_tabSize = size;
    setTabStopDistance(font().pointSize() * m_tabSize);
}

void
GtpyScriptView::replaceTabsBySpaces(bool enable)
{
    m_replaceTabBySpaces = enable;

    if (m_replaceTabBySpaces)
    {
        QString spaces = " ";
        searchAndReplace("\t", spaces.repeated(m_tabSize));
    }
}


void
GtpyScriptView::searchHighlighting(const QString& searchText,
                                     bool moveToNextFound)
{
    m_lastSearch = searchText;

    if (searchText.isEmpty() || isReadOnly())
    {
        removeSearchHighlighting();
        return;
    }

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    auto color = !gtApp->inDarkMode() ?
                QColor{Qt::green}.lighter(160) :
                gt::gui::color::code_editor::highlightLine();
#else
    auto color = QColor{Qt::green}.lighter(160);
#endif

    QList<QTextEdit::ExtraSelection> extraSelections;

    // search forwards
    auto cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor = document()->find(searchText, cursor);

    while (!cursor.isNull())
    {
        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(color);
        selection.cursor = cursor;
        extraSelections.append(selection);

        cursor = document()->find(searchText, cursor);
    }

    // search backwards
    cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor = document()->find(searchText, cursor, QTextDocument::FindBackward);

    while (!cursor.isNull())
    {
        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(color);
        selection.cursor = cursor;
        extraSelections.append(selection);

        cursor = document()->find(searchText, cursor,
                                  QTextDocument::FindBackward);
    }

    if (moveToNextFound && !extraSelections.isEmpty())
    {
        setTextCursor(extraSelections.first().cursor);
    }

    setExtraSelections(extraSelections);

    lineHighlighting();
}

void
GtpyScriptView::removeSearchHighlighting()
{
    m_searchActivated = false;

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
GtpyScriptView::textSearchingActivated()
{
    m_searchActivated = true;
}

void
GtpyScriptView::moveCursorToNextFound()
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
GtpyScriptView::moveCursorToLastFound()
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
GtpyScriptView::keyPressEvent(QKeyEvent* event)
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

            case Qt::Key_R:
                cursor = textCursor();
                text = cursor.selectedText();
                emit replaceShortcutTriggered(text);
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

                    tabSize = tabSize % m_tabSize;

                    QString spaces = " ";
                    spaces = spaces.repeated(m_tabSize - tabSize);

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

    if (m_searchActivated)
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
GtpyScriptView::dragEnterEvent(QDragEnterEvent* event)
{
    bool valid = validateDrop(event->mimeData());

    if (valid)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void
GtpyScriptView::dragMoveEvent(QDragMoveEvent* event)
{
    bool valid = validateDrop(event->mimeData());

    if (valid)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void
GtpyScriptView::dropEvent(QDropEvent* event)
{
    QString gtObjectClassName = GtObject::staticMetaObject.className();

    if (!event->mimeData()->formats().contains(gtObjectClassName))
    {
        event->ignore();
        return;
    }

    const QString droppedContent = event->mimeData()->data(gtObjectClassName);

    if (!droppedContent.isEmpty())
    {
        GtObjectMemento droppedM(droppedContent.toUtf8());

        if (!droppedM.isNull())
        {
            QString gtCalculatorClassName =
                GtCalculator::staticMetaObject.className();

            bool castable = droppedM.canCastTo(
                                gtCalculatorClassName, gtObjectFactory);

            if (castable)
            {
                GtCalculator* calc =
                    droppedM.restore<GtCalculator*>(gtObjectFactory,
                                                    true);

                if (calc)
                {
                    emit calculatorDropped(calc);
                    event->accept();
                    return;
                }
            }
        }
    }

    event->ignore();
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
GtpyScriptView::functionCallPyCode(const QString& newVal,
                                     const QString& functionName,
                                     const QString& pyObjName)
{
    return pyObjName + "." + functionName + "(" + newVal + ")";
}

void GtpyScriptView::commentLine(bool commentOut)
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

void GtpyScriptView::commentOutBlock()
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

void GtpyScriptView::handleCompletion()
{
    QTextCursor c = this->textCursor();
    QRect cr = cursorRect(c);

    m_cpl->handleTabCompletion(c, cr);
}

void GtpyScriptView::resetErrorMessage()
{
    m_errorMessage = QString();
}

bool GtpyScriptView::isCurrentLineCommentedOut()
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

    cursor.movePosition(QTextCursor::StartOfLine,
                        QTextCursor::KeepAnchor);

    QString line = cursor.selectedText();

    // match leading tabs
    QRegularExpression regExp(QStringLiteral("^\\s+"));
    QRegularExpressionMatch match = regExp.match(line);

    QString newLine = match.captured();

    // match wheter a definiton starts (e.g. def func():)
    regExp.setPattern(QStringLiteral(":\\s*\\Z"));
    match  = regExp.match(line);

    // append tab
    if (match.hasMatch())
    {
        if (m_replaceTabBySpaces)
        {
            QString spaces = " ";
            newLine += spaces.repeated(m_tabSize);
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
    tabSpaces = tabSpaces.repeated(m_tabSize);

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

bool
GtpyScriptView::validateDrop(const QMimeData* droppedData)
{
    QString gtObjectClassName = GtObject::staticMetaObject.className();

    if (!droppedData->formats().contains(gtObjectClassName))
    {
        return false;
    }

    const QString droppedContent = droppedData->data(gtObjectClassName);

    if (!droppedContent.isEmpty())
    {
        GtObjectMemento droppedM(droppedContent.toUtf8());

        if (!droppedM.isNull())
        {
            QString gtCalculatorClassName =
                GtCalculator::staticMetaObject.className();

            bool castable = droppedM.canCastTo(
                                gtCalculatorClassName, gtObjectFactory);

            if (castable)
            {
                return true;
            }
        }
    }

    return false;
}
