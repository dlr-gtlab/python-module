/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_console.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_CONSOLE_H
#define GTPY_CONSOLE_H

#include <QTextEdit>

#include "gtpy_contextmanager.h"

class GtpyCompleter;
class GtPyCompleterModel;

/**
 * @brief The GtpyConsole class
 */
class GtpyConsole: public QTextEdit
{
    Q_OBJECT

public:
    /**
     * @brief GtPythonScriptingConsole
     * @param contextId It of the context in which the scripts lines written
     * in the console are evaluated.
     * @param parent Parent of the editor.
     */
    GtpyConsole(int contextId, QWidget* parent);

    /**
     * @brief Sets the text of command prompt of the console.
     * @param commandPrompt Text of command prompt.
     */
    void setCommandPrompt(const QString& commandPrompt);

public slots:
    /**
     * @brief Enables the registration of a context whose output is displayed
     *  on this console. No input will be sent to this context.
     * @param contextId Id of the context which should be added.
     */
    void showAdditionalContextOutput(int contextId);

    /**
     * @brief Removes the context with the specified ID from the list of
     * contexts for displaying output on this console.
     * @param contextId Id of the context which should be removed.
     */
    void removeAdditionalContextOutput(int contextId);

    /**
     * @brief Receives error messages and appends them to the console.
     * @param message Error message which should be shown.
     * @param contextId Id of the python context that triggered the error
     * message.
     */
    void stdErr(const QString& message, int contextId);

    /**
     * @brief Clears the console text.
     */
    void clearConsole();

protected:
    /**
     * @brief keyPressEvent Will be called after user pressed a key.
     * @param e Event which was send by pressing a key.
     */
    void keyPressEvent (QKeyEvent* e) Q_DECL_OVERRIDE;

private:

    static const QRegularExpression RE_KEYBOARD_INTERRUPT;
    static const QRegularExpression RE_ERROR_LINE;

    /// History of Commands
    QStringList m_history;

    /// Current Position in History
    int m_historyPosition;

    /// Comand Prompt
    QString m_commandPrompt;

    /// Default Command Prompt
    QString m_defaultPrompt;

    /// Current Multiline Code
    QString m_currentMultiLineCode;

    /// Output Message
    QString m_stdOut;

    /// Error Message
    QString m_stdErr;

    /// Python Context Manager
    GtpyContextManager* m_python;

    /// Python Context id
    int m_contextId;

    /// Default Text Character Format
    QTextCharFormat m_defaultTextCharacterFormat;

    /// Python Completer
    GtpyCompleter* m_cpl;

    /// Python Completer Model
    GtPyCompleterModel* m_model;

    QList<int> m_additionalContextOutput;

    /// Cursor Position
    int m_cursorPosition;

    /**
     * @brief Executes current input.
     * @param storeOnly True if multiline input.
     */
    void executeLine(bool storeOnly);

    /**
     * @brief Executes the given python code.
     * @param code Python code.
     */
    void executeCode(const QString& code);

    /**
     * @brief Returns the position of the command prompt.
     * @return Position of the command prompt
     */
    int commandPromptPosition();

    /**
     * @brief Adds input to history list.
     */
    void changeHistory();

    /**
     * @brief Verifies the selection before deletion.
     * @return False if selection is invalid. (e.g. command prompt is selectet)
     */
    bool verifySelectionBeforeDeletion();

    /**
     * @brief moveCursorBehindPrompt Moves cursor behind prompt. Unselects text
     *  that is in front of the prompt
     * @param textCursor The cursor that will be moved
     */
    void moveCursorBehindPrompt(QTextCursor& textCursor);

    /**
     * @brief Handles the selected completion.
     */
    void handleCompletion();

    /**
     * @brief Sets the Font of console text.
     * @param color Color of conosle text.
     * @param bold If blod is true, it sets the font's weight to blod.
     */
    void setCurrentFont(const QColor& color = QColor(0,0,0), bool bold = false);

    /**
     * @brief checks whether the text in the console should be/continued to be
     * evaluated in multiline-mode or not
     * @param tabCount indicates how many tabs the new line should be indented
     * @return true if multiline-mode should be used
     */
    bool storeLine(int* tabCount);

    /**
     * @brief chekcs if the error message of the python exception
     * 'KeyboardInterrupt' was send to the console. If so it replaces it with a
     * propper message
     */
    void hideKeyboardInterruptException();

private slots:

    /**
     * @brief Receives messages and appends them to the console.
     * @param message Message which should be shown.
     * @param contextId Id of the python context that triggered the message.
     */
    void stdOut(const QString& message, int contextId);

    /**
     * @brief Sets the cursor to the end of the console.
     * @param contextId Id of the python context that triggered the cursor move.
     */
    void cursorToEnd(int contextId);

    /**
     * @brief Appends a new command prompt to the end of the console.
     * @param contextId Id of the python context that triggered the
     * appending of the promt.
     * @param storeOnly If storeOnly is true, it appends the multiline prompt.
     */
    void appendCommandPrompt(int contextId, bool storeOnly = false);

    /**
     * @brief Append new line and command prompt after execution.
     * @param contextId Id of the python context in which the code was executed.
     */
    void onCodeExecuted(int contextId);

    /**
     * @brief Inserts the selected completion.
     */
    void insertCompletion();

    /**
     * @brief Inserts message to console.
     * @param message Message that should be inserted.
     */
    void consoleMessage(const QString& message);

};

#endif // GTPY_CONSOLE_H
