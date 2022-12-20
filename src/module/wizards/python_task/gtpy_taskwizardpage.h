/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskwizardpage.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_TASKWIZARDPAGE_H
#define GTPY_TASKWIZARDPAGE_H

#include <QModelIndex>

#include "gtpy_task.h"

class QSignalMapper;
class GtProcessFilterModel;
class GtpyTaskStyleModel;
class GtpyTaskTreeView;
class GtpyObjectModel;

#include "gtpy_abstractscriptingwizardpage.h"

/**
 * @brief The GtpyTaskWizardPage class
 */
class GtpyTaskWizardPage : public GtpyAbstractScriptingWizardPage
{
    Q_OBJECT

public:
    /**
     * @brief GtpyTaskWizardPage
     */
    Q_INVOKABLE explicit GtpyTaskWizardPage();

protected:
    /**
     * @brief Will be called after script evaluation. It updates the task tree
     * view.
     * @param success True if evaluation was successful, otherwise false.
     */
    virtual void endEval(bool success) Q_DECL_OVERRIDE;

private:
    /**
     * @brief Sets python script from task instance to editor.
     */
    virtual void initialization() Q_DECL_OVERRIDE;

    /**
     * @brief Sets script from editor to task instance.
     * @return True if settig script to calculator instance was successful.
     */
    virtual bool validation() Q_DECL_OVERRIDE;

    /**
     * @brief Saves the script into the task instance.
     */
    virtual void saveScript() Q_DECL_OVERRIDE;

    /**
     * @brief Returns the uuid of the restored GtpyScriptCalculator.
     * @return uuid of the restored GtpyScriptCalculator
     */
    virtual QString componentUuid() const Q_DECL_OVERRIDE;

    /**
     * @brief Sets the object name of the process component m_task to the
     * given name.
     * @param name New name of the process component m_task
     */
    virtual void setComponentName(const QString& name) Q_DECL_OVERRIDE;

    /**
     * @brief Creates a new settings instance containing the current defined
     * settings and returns it.
     * @return A new settings instace containing the current settings.
     */
    virtual GtpyEditorSettings* createSettings() Q_DECL_OVERRIDE;

    /**
     * @brief Passes the settings defined in the given pointer pref to the
     * task.
     * @param pref Current editor settings.
     */
    virtual void saveSettings(GtpyEditorSettings* pref) Q_DECL_OVERRIDE;

    /**
     * @brief mapToSource
     * @param index
     * @return
     */
    QModelIndex mapToSource(const QModelIndex& index);

    /**
     * @brief Opens the calculator Wizard and edits the Python code which
     * represents the settings.
     * @param calc The calculator to be configured.
     */
    void configCalculator(GtCalculator* calc);

    /**
     * @brief Opens the Calculator selection menu. When a new calculator was
     * generated the Python code which represents the settings will be added
     * to the script.
     */
    void addCalculator();

    /**
     * @brief Updates list of last used process elements.
     * @param str Last used process element class name.
     */
    void updateLastUsedElementList(const QString& str);

    /**
     * @brief Uses GtpyCodeGenerator to create the Python code representing the
     * calculator settings and inserts it into the script.
     * @param calc Edit calculator object.
     * @param before Memento object of the calculator before editing.
     */
    void insertConstructor(GtCalculator* calc);

    /// Task.
    QPointer<GtpyTask> m_task;

    /// Task tree view.
    GtpyTaskTreeView* m_treeView;

    /// Calculator object model.
    GtpyObjectModel* m_calcModel;

    /// Styled model.
    GtpyTaskStyleModel* m_styledModel;

    /// Filter model.
    GtProcessFilterModel* m_filterModel;

    /// Mapper for action signals.
    QSignalMapper* m_actionMapper;

    /// Calculator object names identified by UUIDs
    QHash<QString, QString> m_calcIds;

    static const QString ARROW_LEFT;
    static const QString ARROW_RIGHT;

private slots:
    /**
     * @brief Opens the wizard of the calculator with the given index.
     * @param index Model index of calculator.
     */
    void onDoubleClicked(const QModelIndex& index);

    /**
     * @brief Deletes the process elements at the given indexes.
     * @param indexes Indexes of calculators.
     */
    void deleteProcessElements(const QList<QModelIndex>& indexList);

    /**
     * @brief Renames the calculator in the Python script.
     * @param className Class name of the calculator.
     * @param oldName Old name of the renamed calculator.
     * @param newName New name of the renamed calculator.
     */
    void onProcessComponentRenamed(const QString& className,
                                   const QString& oldName,
                                   const QString& newName);

    /**
     * @brief Opens the menu for adding a calculator.
     */
    void addElement();

    /**
     * @brief Identifies the action and opens the corresponding wizard.
     * @param obj Triggered aciton.
     */
    void actionTriggered(QObject* obj);

    /**
     * @brief Enables name observation for the calculator specified in child.
     * @param child Appended calculator.
     * @param parent Parent of new calculator.
     */
    void calculatorAppendedToTask(GtObject* child, GtObject* parent);

    /**
     * @brief Edits the script after changing the object name of a calculator.
     * @param name New object name.
     */
    void calculatorNameChanged(const QString& name);

    /**
     * @brief Removes calculator uuid and object name from m_calcIds.
     * @param obj Destroyed object.
     */
    void calculatorDestroyed(QObject* obj);

    /**
     * @brief Inserts the python code of the dropped calculator in the editor.
     * @param calc Dropped calculator.
     */
    void onCalculatorDropReceived(GtCalculator* calc);
};
#endif // GTPY_TASKWIZARDPAGE_H
