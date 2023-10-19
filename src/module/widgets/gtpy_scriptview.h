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

#include "gt_calculator.h"
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

    /**
     * @brief Inserts given text to current cursor position.
     * @param text Text to be inserted.
     */
    void insertToCurrentCursorPos(const QString& text);

//    /**
//     * @brief Replaces the value of function call of the function named
//     * functionName into the block delimited by header and caption.
//     * @param header Upper limit of the block.
//     * @param caption Lower limit of the block.
//     * @param newVal New value that has to set.
//     * @param functionName Name of function
//     * @param pyObjName Name of python object
//     */
//    void replaceIntoBlock(const QString& header, const QString& caption,
//                          const QString& newVal, const QString& functionName,
//                          const QString& pyObjName);

    /**
     * @brief Replaces old header and old caption of a block with given new
     * header and new caption.
     * @param oldHeader Old header string
     * @param newHeader New header string
     * @param oldCaption Old caption string
     * @param newCaption New caption string
     */
    void replaceBlockHeaders(const QString& oldHeader, const QString& newHeader,
                             const QString& oldCaption, const QString& newCaption);

    /**
     * @brief Searchs for the given regular expression and replaces it
     * with replaceBy.
     * @param searchRegExp Regular expression that should be matched.
     * @param replaceBy Text to insert.
     * @param all If it is true, all strings in the document will be replaced.
     * Otherwise it only replaces the first one found.
     */
    void searchAndReplace(const QRegExp& searchRegExp, const QString& replaceBy,
                          bool all = true);
    /**
     * @brief Searchs for the given string searchFor and replaces it
     * with replaceBy.
     * @param searchFor Text to replace.
     * @param replaceBy Text to insert.
     * @param all If it is true, all strings in the document will be replaced.
     * Otherwise it only replaces the first one found.
     */
    void searchAndReplace(const QString& searchFor, const QString& replaceBy,
                          bool all = true);

    /**
     * @brief Set drops accepted or not.
     * @param accept Determines whether drops are accepted or not.
     */
    void acceptCalculatorDrops(bool accept);

    /**
     * @brief Returns current cursor postion.
     * @return Current cursor position.
     */
    int cursorPosition();

    /**
     * @brief Sets the cursor to the given position.
     * @param pos New cursor position.
     */
    void setCursorPosition(int pos);

    int verticalSliderPos();

    void setVerticalSliderPos(int pos);

    /**
     * @brief Sets the tab size to the given size.
     * @param size Tab size
     */
    void setTabSize(int size);

    void replaceTabsBySpaces(bool enable = true);

public slots:
    /**
     * @brief Searchs for the given text and highlights this.
     * @param searchText Text which should be searched.
     * @param moveToNextFound Whether cursor should move to the next found.
     */
    void searchHighlighting(const QString& searchText,
                            bool moveToNextFound = true);

    /**
     * @brief Removes the highlighting of a searched word or text part.
     */
    void removeSearchHighlighting();

    /**
     * @brief Sets m_SearchActivated to true.
     */
    void textSearchingActivated();

    /**
     * @brief Moves cursor to the next found of searching.
     */
    void moveCursorToNextFound();

    /**
     * @brief Moves cursor to the last found of searching.
     */
    void moveCursorToLastFound();

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

    /**
     * @brief Accept the drag enter if the mimedata contains a calculator
     * object.
     * @param event Raised event.
     */
    virtual void dragEnterEvent(QDragEnterEvent* event) override;

    /**
     * @brief Accept the drag move if the mimedata contains a calculator object.
     * @param event Raised event.
     */
    virtual void dragMoveEvent(QDragMoveEvent* event) override;

    /**
     * @brief Accept the drop if the mimedata contains a calculator object.
     * @param event Raised event.
     */
    virtual void dropEvent(QDropEvent* event) override;

private slots:
    /**
     * @brief Highlights the current line.
     */
    void lineHighlighting();

    /**
     * @brief Highlights the given line as error line (red).
     * @param codeLine number of line which should be highligted
     * @param contextId Id of the context in which the error has been triggered.
     */
    void highlightErrorLine(int codeLine,
                            int contextId);

    /**
     * @brief Receives the error messages.
     * @param message that has been send
     * @param contextId Id of the context in which the error has been triggered.
     */
    void appendErrorMessage(const QString& message, int contextId);

    /**
     * @brief Sets the number of error line to invalid value (-1).
     */
    void resetErrorLine();

    /**
     * @brief Inserts the selected completion into the text editor.
     * @param index of completion
     */
    void insertCompletion();

private:
    /// Pointer to cmpleter
    GtpyCompleter* m_cpl;

    /// Number of error line
    int m_errorLine;

    /// Whether the text search is activated
    bool m_searchActivated;

    /// Text that was last searched for
    QString m_lastSearch;

    /// Error message
    QString m_errorMessage;

    /// Python Context id
    int m_contextId;

    /// Tab size
    int m_tabSize;

    bool m_replaceTabBySpaces;

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

    /**
     * @brief The line in which the cursor is currently located is
     * commented out or commented out. (Python comment character "#")
     * @param commentOut true if "#" has to insert
     */
    void commentLine(bool commentOut);

    /**
     * @brief Comments out the selected block.
     */
    void commentOutBlock();

    /**
     * @brief Calls completer to handle completion.
     */
    void handleCompletion();

    /**
     * @brief Sets error message variable to empty string.
     */
    void resetErrorMessage();

    /**
     * @brief Checks whether the line in which the cursor is currently located
     *  is commented out.
     * @return True if current line is commented out.
     */
    bool isCurrentLineCommentedOut();

    /**
     * @brief Checks whether the line in which the cursor is currently located
     * is highlighted.
     * @return True if current line is highlighted.
     */
    bool isCurrentLineHighlighted();

    /**
     * @brief Removes the highlighting of the current line.
     */
    void removeCurrentLineHighlighting();

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

    /**
     * @brief Validates if the given mimedata contains a calculator object.
     * @param droppedData Dropped mimedata.
     * @return True, if the given mimedata contains a calculator object.
     */
    bool validateDrop(const QMimeData* droppedData);

signals:
    /**
     * @brief Will emited if eval shortcut (ctrl+E) has been received.
     */
    void evalShortcutTriggered();

    /**
     * @brief Will emited if find shortcut (ctrl+F) has been received.
     * @param text Selected text.
     */
    void searchShortcutTriggered(const QString& text);

    void calculatorDropped(GtCalculator* calc);
};

#endif // GTPYSCRIPTVIEW_H
