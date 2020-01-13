/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_console.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "iostream"

#include <QAbstractItemView>
#include <QToolTip>
#include <QClipboard>
#include <QApplication>
#include <QRegularExpression>

#include "gt_application.h"
#include "gt_command.h"
#include "gt_pyhighlighter.h"
#include "gt_project.h"

#include "gtpy_completer.h"

#include "gtpy_console.h"


const QRegularExpression GtpyConsole::RE_KEYBOARD_INTERRUPT
    ("Traceback.*\\n(\\s*File.*\\n)+(.*\\n)?KeyboardInterrupt");
const QRegularExpression GtpyConsole::RE_ERROR_LINE
    ("\"<string>\", line \\d+");


GtpyConsole::GtpyConsole(GtpyContextManager::Context type,
                         QWidget* parent) :
    QTextEdit(parent), m_python(Q_NULLPTR), m_defaultPrompt("> ")
{
    setFrameStyle(QFrame::NoFrame);

    setCommandPrompt("GTlab");
    const QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    setFont(font);

    GtPyHighlighter* highlighter = new GtPyHighlighter(this->document());

    Q_UNUSED(highlighter)

    m_contextType = type;

    m_defaultTextCharacterFormat = currentCharFormat();
    m_historyPosition = 0;

    m_cpl = new GtpyCompleter(m_contextType, this);

    m_python = GtpyContextManager::instance();

    connect(m_python, SIGNAL(pythonMessage(QString,
                     GtpyContextManager::Context)), this,
            SLOT(stdOut(const QString&, GtpyContextManager::Context)));
    connect(m_python, SIGNAL(errorMessage(QString,
                                          GtpyContextManager::Context)),
            this, SLOT(stdErr(const QString&, GtpyContextManager::Context)));
    connect(m_python, SIGNAL(startedScriptEvaluation(
                                 GtpyContextManager::Context)), this,
            SLOT(cursorToEnd(GtpyContextManager::Context)));
    connect(m_python, SIGNAL(scriptEvaluated(GtpyContextManager::Context)),
            this, SLOT(onCodeExecuted(GtpyContextManager::Context)));

    connect(m_cpl, SIGNAL(activated(QModelIndex)), this,
            SLOT(insertCompletion()));

    clearConsole();

    const int tabStop = 4;
    QFontMetrics metrics(font);
    setTabStopWidth(tabStop * metrics.width(' '));
}

void
GtpyConsole::setCommandPrompt(const QString& commandPrompt)
{
    m_commandPrompt = commandPrompt;
    m_commandPrompt.append(m_defaultPrompt);
    clearConsole();
}

void
GtpyConsole::showAdditionalContextOutput(
        GtpyContextManager::Context context)
{
   if (!m_additionalContextOutput.contains(context))
   {
       m_additionalContextOutput.append(context);
   }
}

void
GtpyConsole::stdErr(const QString& message,
                                 GtpyContextManager::Context type)
{
    if (m_contextType == type || m_additionalContextOutput.contains(type))
    {
        setTextColor(QColor(214, 0, 0));

        m_stdErr += message;
        int idx;

        while ((idx = m_stdErr.indexOf('\n')) != -1)
        {
            consoleMessage(m_stdErr.left(idx));
            std::cerr << m_stdErr.left(idx).toLatin1().data() << std::endl;
            m_stdErr = m_stdErr.mid(idx + 1);
        }
    }
}

void
GtpyConsole::clearConsole()
{
    QTextEdit::clear();
    appendCommandPrompt(m_contextType);
}

void
GtpyConsole::keyPressEvent(QKeyEvent* e)
{
    if (m_cpl == Q_NULLPTR)
    {
        QTextEdit::keyPressEvent(e);
    }

    QToolTip::showText(QPoint(), "");

    if (m_cpl->getPopup()->isVisible())
    {
        switch (e->key())
        {
            case Qt::Key_Enter:
            case Qt::Key_Return:

                insertCompletion();
                e->accept();
                return;

            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Left:
                QTextEdit::keyPressEvent(e);
                handleCompletion();
                return;

            case Qt::Key_Right:
                QTextEdit::keyPressEvent(e);
                handleCompletion();
                return;

            case Qt::Key_Backtab:

                e->ignore();
                return;

            default:
                break;
        }
    }

    bool        eventHandled = false;
    QTextCursor textCursor   = this->textCursor();

    int key = e->key();

    switch (key)
    {
        case Qt::Key_Left:

            if (textCursor.position() <= commandPromptPosition())
            {
                eventHandled = true;
            }
            break;

        case Qt::Key_Up:

            if (m_historyPosition > 0)
            {
                m_historyPosition--;
                changeHistory();
            }

            eventHandled = true;
            break;

        case Qt::Key_Down:

            if (m_historyPosition + 1 < m_history.count())
            {
                m_historyPosition++;
                changeHistory();
            }

            eventHandled = true;
            break;

        case Qt::Key_Return:

            if (this->isReadOnly())
            {
                break;
            }

            executeLine(e->modifiers() & Qt::ShiftModifier);
            eventHandled = true;
            break;

        case Qt::Key_Backspace:

            if (this->isReadOnly())
            {
                break;
            }

            if (textCursor.hasSelection())
            {
                bool deletionAllowed = verifySelectionBeforeDeletion();

                if (!deletionAllowed)
                {
                    eventHandled = true;
                }

            }
            else
            {
                if (textCursor.position() <= commandPromptPosition())
                {
                    eventHandled = true;
                }
            }
            break;

        case Qt::Key_Delete:

            if (this->isReadOnly())
            {
                break;
            }

            if (textCursor.hasSelection())
            {
                bool deletionAllowed = verifySelectionBeforeDeletion();

                if (!deletionAllowed)
                {
                    eventHandled = true;
                }
            }
            else
            {
                if (textCursor.position() < commandPromptPosition())
                {
                    eventHandled = true;
                }
            }
            break;

        /*copies selected text from console to clipboard*/
        case Qt::Key_C:

            if(e->modifiers() & Qt::ControlModifier)
            {
                if (textCursor.hasSelection())
                {
                    QTextEdit::copy();

                    QClipboard* clipboard = QApplication::clipboard();
                    QString selectedStr = clipboard->text().prepend("\n");

                    QRegularExpression matchPrompt("\n[\\w\\.]*[^\\s]" +
                                                   m_defaultPrompt);

                    selectedStr.replace(matchPrompt, "\n");
                    selectedStr.remove(0, 1);

                    clipboard->setText(selectedStr);

                    eventHandled = true;
                }
            }
            break;

        /*pastes code from clipboard to console*/
        case Qt::Key_V:

            if(e->modifiers() & Qt::ControlModifier)
            {
                if (this->isReadOnly())
                {
                    break;
                }

                bool deletionAllowed = verifySelectionBeforeDeletion();

                if (!deletionAllowed)
                {
                    moveCursorBehindPrompt(textCursor);
                }

                QClipboard* clipboard = QApplication::clipboard();

                QStringListIterator clipboardIterator =
                        clipboard->text().split("\n");

                while (clipboardIterator.hasNext())
                {
                    textCursor.insertText(clipboardIterator.next());

                    if (clipboardIterator.hasNext())
                    {
                        executeLine(true);
                    }
                }

                eventHandled = true;
            }
            break;

        default:

            if (key >= Qt::Key_Space && key <= Qt::Key_division)
            {
                bool deletionAllowed = verifySelectionBeforeDeletion();

                if (!deletionAllowed)
                {
                    moveCursorBehindPrompt(textCursor);
                }
            }
    }

    if (eventHandled)
    {
        m_cpl->getPopup()->hide();
        e->accept();
    }
    else
    {
        QTextEdit::keyPressEvent(e);
        QString text = e->text();

        if (!text.isEmpty())
        {
            handleCompletion();
        }
        else
        {
            m_cpl->getPopup()->hide();
        }

        eventHandled = true;
    }
}

void
GtpyConsole::executeLine(bool storeOnly)
{
    QTextCursor textCursor = this->textCursor();
    textCursor.movePosition(QTextCursor::End);

    textCursor.setPosition(commandPromptPosition());
    textCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    QString code = textCursor.selectedText();

    if (code.endsWith(" "))
    {
        code.truncate(code.length() - 1);
    }

    if (!code.isEmpty())
    {
        m_history << code;
        m_historyPosition = m_history.count();
    }

    if (m_currentMultiLineCode.isEmpty())
    {
        m_currentMultiLineCode = code;
    }
    else
    {
        m_currentMultiLineCode = m_currentMultiLineCode + "\n" + code;
    }

    if (!storeOnly)
    {
        executeCode(m_currentMultiLineCode);
        m_currentMultiLineCode = "";
    }
    else
    {
        appendCommandPrompt(m_contextType, storeOnly);
    }
}

void
GtpyConsole::executeCode(const QString& code)
{
    QTextCursor cursor = this->textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);

    m_cursorPosition = this->textCursor().position();

    m_stdOut = "";
    m_stdErr = "";

    GtCommand com = gtApp->startCommand(gtApp->currentProject(),
                                        "Python Command");

    if (code.indexOf("\n") != -1)
    {
        m_python->evalScript(m_contextType, code, true,
                             GtpyContextManager::EvalFile);
    }
    else
    {
        m_python->evalScript(m_contextType, code, true,
                             GtpyContextManager::EvalSingleString);
    }

    gtApp->endCommand(com);
}

void
GtpyConsole::consoleCut()
{
    bool deletionAllowed = verifySelectionBeforeDeletion();

    if (deletionAllowed)
    {
        QTextEdit::cut();
    }
}


int
GtpyConsole::commandPromptPosition()
{
    QTextCursor textCursor = this->textCursor();
    textCursor.movePosition(QTextCursor::End);

    return textCursor.block().position() + m_commandPrompt.length();
}

void
GtpyConsole::changeHistory()
{
    QTextCursor textCursor = this->textCursor();
    textCursor.movePosition(QTextCursor::End);
    textCursor.setPosition(commandPromptPosition(), QTextCursor::KeepAnchor);

    textCursor.insertText(m_history.value(m_historyPosition));

    textCursor.movePosition(QTextCursor::End);
    setTextCursor(textCursor);
}

bool
GtpyConsole::verifySelectionBeforeDeletion()
{
    bool deletionAllowed = true;

    QTextCursor textCursor = this->textCursor();

    int commandPromptPosition = this->commandPromptPosition();
    int selectionStart = textCursor.selectionStart();
    int selectionEnd = textCursor.selectionEnd();

    if (textCursor.hasSelection())
    {
        if (selectionStart < commandPromptPosition ||
                selectionEnd < commandPromptPosition)
        {
            if (selectionStart > selectionEnd)
            {
                int tmp         = selectionEnd;
                selectionEnd    = selectionStart;
                selectionStart  = tmp;
            }

            if (selectionEnd < commandPromptPosition ||
                    selectionStart < commandPromptPosition)
            {
                deletionAllowed = false;
            }
            else
            {
                selectionStart = commandPromptPosition;
                textCursor.setPosition(selectionStart);
                textCursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
                setTextCursor(textCursor);
            }
        }

    }
    else
    {
        if (textCursor.position() < commandPromptPosition)
        {
            deletionAllowed = false;
        }
    }

    return deletionAllowed;
}

void
GtpyConsole::moveCursorBehindPrompt(QTextCursor& textCursor)
{
    int promptPosition = this->commandPromptPosition();
    int cursorPosition = textCursor.position();
    int anchorPosition = textCursor.anchor();

    if (!textCursor.hasSelection())
    {
        if (cursorPosition < promptPosition)
        {
            textCursor.setPosition(promptPosition,
                                   QTextCursor::MoveAnchor);
        }
    }
    else
    {
        if (anchorPosition < promptPosition)
        {
            textCursor.setPosition(promptPosition,
                                   QTextCursor::MoveAnchor);
            textCursor.setPosition(cursorPosition,
                                   QTextCursor::KeepAnchor);
        }

        if (cursorPosition < promptPosition)
        {
            textCursor.setPosition(promptPosition,
                                   QTextCursor::KeepAnchor);

        }
    }

    setTextCursor(textCursor);
}

void
GtpyConsole::handleCompletion()
{
    QTextCursor c = this->textCursor();
    QRect cr = cursorRect(c);

    m_cpl->handleTabCompletion(c, cr);
}

void
GtpyConsole::setCurrentFont(const QColor& color, bool bold)
{
    QTextCharFormat charFormat(m_defaultTextCharacterFormat);

    QFont font(charFormat.font());
    font.setBold(bold);
    charFormat.setFont(font);

    QBrush brush(charFormat.foreground());
    brush.setColor(color);
    charFormat.setBackground(brush);

    setCurrentCharFormat(charFormat);
}

void
GtpyConsole::hideKeyboardInterruptException()
{
    QString error = this->toPlainText();

    if (error.indexOf(RE_KEYBOARD_INTERRUPT) == -1)
    {
        return;
    }

    QRegularExpressionMatch matchError = RE_KEYBOARD_INTERRUPT.match(error);
    QRegularExpressionMatch matchLine  = RE_ERROR_LINE.match(
                matchError.captured());

    // extract lineNumber, size of RE_ERROR_LINE until digit is 17
    int lineNumber = matchLine.captured().mid(17).toInt();

    QString output = error;
    QString interruptString;
    interruptString += " --- Interrupted script at line ";
    interruptString += QString::number(lineNumber);
    interruptString += + " --- ";

    output.replace(matchError.captured(), interruptString);

    this->clear();
    this->insertPlainText(output);
}

void
GtpyConsole::stdOut(const QString& message,
                                 GtpyContextManager::Context type)
{
    if (m_contextType == type ||
            (m_additionalContextOutput.contains(type)))
    {
        m_stdOut += message;
        int idx;

        while ((idx = m_stdOut.indexOf('\n')) != -1)
        {
            consoleMessage(m_stdOut.left(idx));
            std::cout << m_stdOut.left(idx).toLatin1().data() << std::endl;
            m_stdOut = m_stdOut.mid(idx + 1);
        }
    }
}

void
GtpyConsole::cursorToEnd(const GtpyContextManager::Context& type)
{
    if (type == m_contextType || m_additionalContextOutput.contains(type))
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
    }
}

void
GtpyConsole::appendCommandPrompt(
    GtpyContextManager::Context type, bool storeOnly)
{
    if (type == m_contextType || m_additionalContextOutput.contains(type))
    {
        if (storeOnly)
        {
            QString temp = m_commandPrompt;

            int size = m_commandPrompt.size() - m_defaultPrompt.size();

            m_commandPrompt.clear();

            for (int i = 0; i < size; i++)
            {
                m_commandPrompt.append(".");
            }

            m_commandPrompt.append(m_defaultPrompt);

            append(m_commandPrompt);

            m_commandPrompt = temp;
        }
        else
        {
            append(m_commandPrompt);
        }

        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
    }
}

void
GtpyConsole::onCodeExecuted(GtpyContextManager::Context type)
{
    if (type == m_contextType || m_additionalContextOutput.contains(type))
    {
        if (!m_stdOut.isEmpty())
        {
            stdOut("\n", m_contextType);
        }

        if (!m_stdErr.isEmpty())
        {
            stdErr("\n", m_contextType);
        }

        bool messageInserted = (textCursor().position() != m_cursorPosition);

        if (messageInserted)
        {
            append(QString());
        }

        appendCommandPrompt(m_contextType);
    }
}

void
GtpyConsole::insertCompletion()
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

void
GtpyConsole::consoleMessage(const QString& message)
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
    append(QString());
    insertPlainText(message);

    hideKeyboardInterruptException();

    setCurrentCharFormat(m_defaultTextCharacterFormat);
}
