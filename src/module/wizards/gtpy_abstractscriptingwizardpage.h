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

#include "gt_processwizardpage.h"
#include "gt_calculator.h"

#include "gtpy_contextmanager.h"
#include "gtpy_editorsettings.h"

class QSplitter;
class QTabWidget;
class QLabel;
class GtObject;
class GtSearchWidget;
class GtAbstractProperty;
class GtProcessComponent;
class GtpyScriptEditor;
class GtpyConsole;
class GtpyScriptRunnable;

/**
 * @brief The GtpyAbstractScriptingWizardPage class
 */
class GT_PYTHON_EXPORT GtpyAbstractScriptingWizardPage :
        public GtProcessWizardPage
{
    Q_OBJECT

public:
    /**
     * @brief GtpyAbstractScriptingWizardPage
     * @param type python context type
     */
    explicit GtpyAbstractScriptingWizardPage(GtpyContextManager::Context type);

    /**
     * @brief ~GtpyAbstractScriptingWizardPage
     */
    virtual ~GtpyAbstractScriptingWizardPage();

    /**
     * @brief Will be called just before page is shown. Adds registered
     * packages to the Python context.
     */
    virtual void initializePage() override final;

    /**
     * @brief Will be called just before page is closed. Removes all added
     * objects from the python context.
     * @return True if validation was successful.
     */
    virtual bool validatePage() override final;

protected:
    /**
     * @brief Will be called after user pressed a key.
     * @param e Event which was send by pressing a key.
     */
    virtual void keyPressEvent(QKeyEvent* e) override;

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
                           QString pyCode);

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
     * @brief Returens the indentation of the given codeLine.
     * @param codeLine The line of code whose indentation should be returned.
     * @return The indentation of the given codeLine.
     */
    QString indentation(const QString& codeLine) const;

    /**
     * @brief Inserts text at current cursor position of editor widget.
     * @param text Text that should be set to editor.
     */
    void insertToCurrentCursorPos(QString text);

    /**
     * @brief Adds a tab containing the given widget to the editor tab widget.
     * @param wid Widget to add to the editor tab widget.
     * @param label Label of the new tab.
     */
    void addTabWidget(QWidget* wid, const QString& label);

    /**
     * @brief showEvalButton
     * @param show
     */
    void showEvalButton(bool show);

    /**
     * @brief Sets drops of calculator instances to accept.
     * @param accept Decides whether drops of calculator instances are
     * accepted or not.
     */
    void acceptCalculatorDrops(bool accept);

    /**
     * @brief Sets the cursor of the calculator to a new line.
     */
    void cursorToNewLine();

    /**
     * @brief Enable or disable the saving functionality.
     * @param enable Whether the saving functionality should be enabled or
     *  disabled.
     */
    void enableSaving(bool enable = true);

    /// Python Context id
    int m_contextId;

    /// Python Context type
    GtpyContextManager::Context m_contextType;

protected slots:
    /**
     * @brief Evaluates the current script from the editor
     * in the Python context of the calculator.
     */
    void evalScript(bool outputToConsole = true);

    /**
     * @brief Evaluates the specified script
     * in the Python context of the calculator.
     */
    void evalScript(const QString& script, bool outputToConsole = true);

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

    /**
     * @brief In this pure virtual function the routine for saving a script
     * must be implemented.
     */
    virtual void saveScript() = 0;

    /**
     * @brief This pure virtual function must return the uuid of the restored
     * process component.
     * @return uuid of the restored process component
     */
    virtual QString componentUuid() const = 0;

    /**
     * @brief In this pure virtual function the object name of the process
     * component must be set to the given name.
     * @param name New name of the process component
     */
    virtual void setComponentName(const QString& name) = 0;

    /**
     * @brief In this pure virtual function a new settings instance must be
     * created and returned.
     * @return A new settings instance.
     */
    virtual GtpyEditorSettings* createSettings() = 0;

    /**
     * @brief In this pure virtual function the settings defined in the
     * given pointer pref must be passed to the process component.
     * @param pref Current editor settings.
     */
    virtual void saveSettings(GtpyEditorSettings* pref) = 0;

    /**
     * @brief Enables of disables the save button.
     * @param enable If true, the Save button is enabled, otherwise it is
     *  disabled.
     */
    void enableSaveButton(bool enable = true);

    /**
     * @brief saveMesssageBox
     */
    void saveMesssageBox();

    /**
     * @brief Sets the window modality of the wizard to non modal. This allows
     * the interaction with the GTlab UI while the wizard is open.
     */
    void setWizardNonModal();

    /**
     * @brief Registers the current size ot the wizard in the
     * GtpyWizardSettings.
     */
    void registerGeometry();

    /**
     * @brief Change the size of the wizard to the size stored in the
     * GtpyWizardSettings.
     * @param uuid Uuid of the task.
     */
    void reloadWizardGeometry();

    /**
     * @brief Loads the packages whose names are stored in m_packageNames.
     * After calling this function the packages are available in the Python
     * context with the id m_contextId.
     */
    void loadPackages();

    /**
     * @brief If the runnable is not null, this functions connects it to the
     * context manager by calling GtpyContextManager::autoDeleteRunnable().
     * After that this function interrupts the execution of the runnable.
     */
    void deleteRunnable();

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

    /// Evaluate shortcut lable
    QLabel* m_shortCutEval;

    /// Interrupt shortcut lable
    QLabel* m_shortCutInterrupt;

    /// Save Button
    QPushButton* m_saveButton;

    /// Save shortcut label
    QLabel* m_shortCutSave;

    /// Console Clear Button
    QPushButton* m_consoleClearButton;

    /// Python Editor
    GtpyScriptEditor* m_editor;

    /// Editor Splitter
    QSplitter* m_editorSplitter;

    /// Separator Widget
    QWidget* m_separator;

    /// Editor Tab Widget
    QTabWidget* m_tabWidget;

    GtpyEditorSettings* m_editorSettings;

    /// Package Names
    QStringList m_packageNames;

    /// Is Evaluating
    bool m_isEvaluating;

    /// Script runnable for evaluation
    QPointer<GtpyScriptRunnable> m_runnable;

    /// Saving the script
    bool m_savingEnabled;

    /// Process component uuid
    QString m_componentUuid;

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
     * @brief Displays a dialog for defining the editor settings.
     */
    void onSettingsButton();

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
     * @brief If no evaluation is running, this function starts the evaluation.
     * Otherwise, it stops the current evaluation.
     */
    void onEvalButtonClicked();

    /**
     * @brief If no evaluation is running, it calls the onEvalButtonClicked()
     * function.
     */
    void onEvalShortCutTriggered();

    /**
     * @brief It saves the current script.
     */
    void onSaveButtonClicked();

    /**
     * @brief Enables the save button.
     */
    void onTextChanged();

    /**
     * @brief Checks if the evaluation was successful.
     */
    void evaluationFinished();

    /**
     * @brief Emits the signal calculatorDropReceived().
     * @param calc Calculator which was dropped.
     */
    void onCalculatorDropped(GtCalculator* calc);

    /**
     * @brief Renames the cloned process component object and replaces the
     * wizard title with the new name.
     * @param title New process component name
     */
    void componentRenamed(const QString& name);

signals:
    /// Calculator was dropped to the editor.
    void calculatorDropReceived(GtCalculator* calc);
};

#endif // GTPY_ABSTRACTSCRIPTINGWIZARDPAGE_H
