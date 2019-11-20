/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractscriptingwizardpage.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_ABSTRACTSCRIPTINGWIZARDPAGE_H
#define GTPY_ABSTRACTSCRIPTINGWIZARDPAGE_H

#include <QPointer>

#include "gtpy_contextmanager.h"
#include "gt_processwizardpage.h"

class GtpyScriptEditor;
class GtpyConsole;
class GtSearchWidget;
class QSplitter;
class GtObject;
class GtAbstractProperty;
class GtProcessComponent;
class QLabel;

/**
 * @brief The GtpyAbstractScriptingWizardPage class
 */
class GtpyAbstractScriptingWizardPage : public GtProcessWizardPage
{
    Q_OBJECT

public:
    /**
     * @brief GtpyAbstractScriptingWizardPage
     * @param type python context type
     */
    GtpyAbstractScriptingWizardPage(GtpyContextManager::Context type);

    /**
     * @brief Will be called just before page is shown. Adds registered
     * packages to the Python context.
     */
    virtual void initializePage() Q_DECL_OVERRIDE Q_DECL_FINAL;

    /**
     * @brief Will be called just before page is closed. Removes all added
     * objects from the python context.
     * @return True if validation was successful.
     */
     virtual bool validatePage() Q_DECL_OVERRIDE Q_DECL_FINAL;

protected:
    /**
     * @brief Will be called after user pressed a key.
     * @param e Event which was send by pressing a key.
     */
    virtual void keyPressEvent(QKeyEvent* e) Q_DECL_OVERRIDE;

    /**
     * @brief Enables to create calculator instances into scripting environment.
     * @param task The calculator children will be append to this task.
     */
    void enableCalculators(GtTask* task);

    /**
     * @brief Will be called after script evaluation. By default the function
     * does nothing.
     * @param success True if evaluation was successful, otherwise false.
     */
    virtual void endEval(bool success);

    /**
     * @brief Adds the given widget next to the editor widget.
     * @param widget Widget that should be placed next to editor.
     * @param stretchFactor Stretch factor of the given widget
     */
    void insertWidgetNextToEditor(QWidget* widget, int index = -1,
                                  int stretchFactor = 1);

    /**
     * @brief Returns the integer value of the default frame style.
     * @return Integer value of the default frame style
     */
    int defaultFrameStyle();

    /**
     * @brief Sets the given text to editor widget.
     * @param text Text that should be set to editor widget.
     */
    void setPlainTextToEditor(const QString& text);

    /**
     * @brief Returns current text in editor widget.
     * @return current text in editor widget
     */
    QString editorText();

    /**
     * @brief Replaces the calculator settings between header and caption with
     * pyCode.
     * @param header Upper limit of the block.
     * @param caption Lower limit of the block.
     * @param pyCode Calculator setting.
     */
    void replaceCalcPyCode(const QString& header, const QString& caption,
                           const QString& pyCode);

    /**
     * @brief Replaces old header and old caption of a block with given new
     * header and new caption.
     * @param oldHeader Old header string
     * @param newHeader New header string
     * @param oldCaption Old caption string
     * @param newCaption New caption string
     */
    void replaceBlockHeaders(const QString& oldHeader,
                             const QString& newHeader,
                             const QString& oldCaption,
                             const QString& newCaption);

    /**
     * @brief Searchs the given regular expression searchFor into editor and
     * replaces it with replaceBy.
     * @param searchFor Regular expression to be replaced.
     * @param replaceBy Text to be inserted.
     * @param all If it is true, all founds in the document will be replaced.
     * Otherwise it replaces only the first one found.
     */
    void searchAndReplaceEditorText(const QRegExp& searchFor,
                                    const QString& replaceBy, bool all);

    /**
     * @brief Searchs the given string searchFor into editor and replaces it
     * with replaceBy.
     * @param searchFor Text to be replaced.
     * @param replaceBy Text to be inserted.
     * @param all If it is true, all strings in the document will be replaced.
     * Otherwise it replaces only the first one found.
     */
    void searchAndReplaceEditorText(const QString& searchFor,
                                    const QString& replaceBy, bool all);
    /**
     * @brief Makes the console visible or invisible.
     * @param visible If true, the console will be visible. Otherwise it will
     * be invisible.
     */
    void setConsoleVisible(bool visible = true);

    /**
     * @brief Enables the evaluation of the script which is written
     * in the editor.
     * @param enable If it is true, the evaluation will be enabled. Otherwise
     * it will be disabled.
     */
    void enableEvaluation(bool enable = true);

    /**
     * @brief Inserts text at current cursor position of editor widget.
     * @param text Text that should be set to editor.
     */
    void insertToCurrentCursorPos(const QString& text);

    /**
     * @brief Converts the property value of the given property to python value
     * and returns it as string.
     * @param prop Property whose value should be converted.
     * @return Value of property as python string
     */
    QString propValToString(GtAbstractProperty* prop);

    /**
     * @brief Set the list of package names.
     * @param packageNames List of package names.
     */
    void setPackageNames(QStringList packageNames);

    /**
     * @brief Enables of disables the save button.
     * @param enable If true, the Save button is enabled, otherwise it is
     *  disabled.
     */
    void enableSaveButton(bool enable = true);

protected slots:
    /**
     * @brief Evaluates the current script from the editor
     * in the Python context of the calculator.
     */
    bool evalScript(bool outputToConsole = true);

private:
    /**
     * @brief Will be called at initialization of wizard page.
     */
    virtual void initialization();

    /**
     * @brief Will be called at validation of wizard page.
     * @return Whether validation was successful.
     */
    virtual bool validation();

    /// Search Widget
    GtSearchWidget* m_searchWidget;

    /// Search Forward Button
    QPushButton* m_forwardButton;

    /// Search Backward Button
    QPushButton* m_backwardButton;

    /// Python Output Console
    GtpyConsole* m_pythonConsole;

    /// Evaluate Script Button
    QPushButton* m_evalButton;

    /// Save Button
    QPushButton* m_saveButton;

    /// Save shortcut label
    QLabel* m_shortCutSave;

    /// Console Clear Button
    QPushButton* m_consoleClearButton;

    /// Python Editor
    GtpyScriptEditor* m_editor;

    QSplitter* m_editorSplitter;

    /// Python Context Type
    GtpyContextManager::Context m_contextType;

    /// Separator Widget
    QWidget* m_separator;

    /// Package Names
    QStringList m_packageNames;

private slots:

    /**
     * @brief Shows a dialog to selecting a Python script
     *  file and imports it into the editor.
     */
    void onImportScript();

    /**
     * @brief Displays a dialog for selecting a folder
     *  to export the current script to.
     */
    void onExportScript();

    /**
     * @brief Sets text to search widget.
     * @param text Text to be set on the search widget.
     */
    void setSearchedText(const QString& text);

    /**
     * @brief Sets the search forward button and the search backward button
     * visible. Notifies the editor that the search is enabled.
     */
    void onSearchEnabled();

    /**
     * @brief Sets the search forward button and the search backward button
     * invisible. Notifies the editor that the search is disabled.
     */
    void onSearchDisabled();

    /**
     * @brief Instructs the editor to continue the current search backward.
     */
    void onSearchBackward();

    /**
     * @brief Instructs the editor to continue the current search.
     */
    void onSearchForward();

    /**
     * @brief Enables and disables forward and backward buttons.
     */
    void onSearchTextEdit();

    /**
     * @brief In this pure virtual function the routine for saving a script
     * must be implemented.
     */
    virtual void onSaveButtonClicked() = 0;
};

#endif // GTPY_ABSTRACTSCRIPTINGWIZARDPAGE_H
