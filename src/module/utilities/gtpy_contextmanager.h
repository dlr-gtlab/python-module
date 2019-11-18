/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_contextmanager.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_CONTEXTMANAGER_H
#define GTPY_CONTEXTMANAGER_H

#include "gt_pythonmodule_exports.h"

#include <QObject>

#include "PythonQtObjectPtr.h"

class GtTask;
class GtpyDecorator;
class GtpyCalculatorFactory;
class GtProject;

/**
 * @brief The GtpyFunction struct
 * Structure for defining the components of a completion.
 */
struct GtpyFunction
{
    QString name;
    QString completion;
    QString toolTip;
    int cursorOffset = 0;
};

/**
 * @brief The GtpyContextManager class
 */
class GT_PYTHON_EXPORT GtpyContextManager: public QObject
{
    Q_OBJECT

    Q_ENUMS(Context)

public:
    ///DON'T RENAME THIS ENUM
    /**
     * @brief The Context enum
     * Identify python contexts.
     */
    enum Context
    {
        BatchContext,
        GlobalContext,
        ScriptEditorContext,
        CalculatorRunContext,
        TaskEditorContext,
        TaskRunContext
    };

    /**
     * @brief The EvalOptions enum
     */
    enum EvalOptions
    {
        EvalFile = Py_file_input,
        EvalSingleString = Py_single_input
    };

    /**
     * @brief Returns instance of context manager based on singleton pattern.
     * @return Instance of calculator helper factory
     */
    static GtpyContextManager* instance();

    /**
     * @brief Used to obtain the Python version currently active
     * @return Major and minorversion of current Python instance (e.g '3.7')
     */
    QString pythonVersion() const;

    /**
     * @brief Evaluates the given script into the given python context.
     * @param type Python context identifier.
     * @param script
     * @param output
     * @param outputToEveryConsol
     * @param option
     * @return
     */
    bool evalScript(const GtpyContextManager::Context& type,
                    const QString& script, bool output = true,
                    bool outputToEveryConsol = true,
                    EvalOptions option = EvalFile);

    /**
     * @brief introspection
     * @param type
     * @param objectname
     * @param appendModules
     * @return
     */
    QMultiMap<QString, GtpyFunction> introspection(
            const GtpyContextManager::Context& type,
            const QString& objectname,
            const bool appendModules = false);

    /**
     * @brief Adds the object obj to the Python context indicated by
     *  type. After calling this function, the object can be referenced by
     *  the specified name.
     * @param type Python context identifier.
     * @param name Reference name in Python context.
     * @param obj Object to add.
     * @param saveName If true, the reference name is stored in the list of
     * added objects. All stored objects will be removed by calling the
     * removeAllAddedObjectes() function.
     * @return Whether the addition was successful.
     */
    bool addObject(const GtpyContextManager::Context& type,
                   const QString& name, QObject* obj, bool saveName = true);

    /**
     * @brief Removes the object with the given name from the Python context
     * indicated by type.
     * @param type Python context identifier.
     * @param name Reference name in Python context.
     * @return Whether the removal was successful.
     */
    bool removeObject(const GtpyContextManager::Context& type,
                      const QString& name);

    /**
     * @brief Removes all objects added by calling the addObject() function from
     * the Python context indicated by type. Note that it only removes the
     * objctes whose reference name is stored in the list of added objects.
     * @param type Python context identifier.
     * @return Whether the removal was successful.
     */
    bool removeAllAddedObjects(const GtpyContextManager::Context& type);

    /**
     * @brief Adds the task object to the Python context indicated by
     *  type.
     * @param type Python context identifier.
     * @param task Task object to add.
     * @return Whether the addition was successful.
     */
    bool addTaskValue(const GtpyContextManager::Context& type, GtTask* task);

    /**
     * @brief Converts QVariant value val to QString value.
     * @param val Value to convert.
     * @return QString value.
     */
    QString qvariantToPyStr(const QVariant& val);

    /**
     * @brief Returns the name of the find child function defined in decorator class.
     * @return Name of find child function as QString.
     */
    QString findChildFuncName();

    /**
     * @brief Returns the name of the set property value function defined in
     * decorator class.
     * @return Name of set property value function as QString.
     */
    QString setPropertyValueFuncName();

    /**
     * @brief Initializes a Python context for each value of the enum Context.
     */
    void initContexts();

protected:
    /**
     * @brief The GtpyContextManager constructor.
     * @param parent Parent object.
     */
    GtpyContextManager(QObject* parent = Q_NULLPTR);

private:
    /// Private static strings
    static const QString LIST_DATATYPE;
    static const QString VARIANT_DATATYPE;
    static const QString FUNCTION_WARNING;
    static const QString TASK_VAR;
    static const QString CALC_FAC_VAR;
    static const QString HELPER_FAC_VAR;

    /**
     * @brief Enables the calculator access for the Python context indicated by
     *  type.
     * @param type Python context identifier.
     */
    void enableCalculatorAccess(
            const GtpyContextManager::Context& type);

    /**
     * @brief Returns a pointer to the Python context indicated by type.
     * @param type Python context identifier.
     * @return Pointer to Python Context.
     */
    PythonQtObjectPtr context(const GtpyContextManager::Context& type);

    /**
     * @brief Returns a pointer to the Python context indicated by type.
     * @param type Python context identifier.
     * @return Pointer to Python Context.
     */
    PythonQtObjectPtr context(int type);

    /**
     * @brief Initializes additional functionalities for the batch context.
     */
    void initBatchContext();

    /**
     * @brief Initializes additional functionalities for the global context.
     */
    void initGlobalContext();

    /**
     * @brief Initializes additional functionalities for the script editor
     * context.
     */
    void initScriptEditorContext();

    /**
     * @brief Initializes additional functionalities for the calculator run
     * context.
     */
    void initCalculatorRunContext();

    /**
     * @brief Initializes additional functionalities for the task editor
     * context.
     */
    void initTaskEditorContext();

    /**
     * @brief Initializes additional functionalities for the task run context.
     */
    void initTaskRunContext();

    /**
     * @brief Enables the use of GTlab logging functions in the Python context
     * indicated by type.
     * @param type Python context identifier.
     * @param enableGtLogging If true, the logging messages are sent to the
     * GTlab application console.
     */
    void enableGtLogging(const GtpyContextManager::Context& type,
                       bool enableGtLogging);

    /**
     * @brief Searches the line number out of an error message.
     * @param message The error message.
     * @return The line number in which an error was detected.
     */
    int lineOutOfMessage(const QString& message);

//    void recursiveSubHelperRegistration(const GtpyContextManager::Context& type,
//                                        const QString& helperClassName,
//                                        const QString& calcName);

    /**
     * @brief introspectObject
     * @param object
     * @return
     */
    QMultiMap<QString, GtpyFunction> introspectObject(PyObject* object);

    /**
     * @brief Used to define extra functions or keywords for the completer
     * @return custom completions
     */
    QMultiMap<QString, GtpyFunction> customCompletions();

    /**
     * @brief Used to obtain the built-in functions of python
     * @return builtin fucntions of python
     */
    QMultiMap<QString, GtpyFunction> builtInCompletions();

    /**
     * @brief Obtains importable modules and sets it to member variable
     */
    void setImportableModulesCompletions();

    /**
     * @brief Sets custom completions and builtin completions to member
     *  variable. Removes duplicates in these
     */
    void setStandardCompletions();

    /**
     * @brief Registers the type converters in PythonQt that implemented in
     * the GtpyTypeConversion class.
     */
    void registerTypeConverters();

    /**
     * @brief Returns the constractor functions for the calculators.
     * @param type Python context identifier.
     * @return Constructor functions for the calculators.
     */
    QMultiMap<QString, GtpyFunction> calculatorCompletions(const Context& type);

    /// Map of Python context
    QMap<int, PythonQtObjectPtr> m_contextMap;

    /// standard completions for pytho completer
    QMultiMap<QString, GtpyFunction> m_standardCompletions;

    /// contains completions of importable modules
    QMultiMap<QString, GtpyFunction> m_importableModulesCompletions;

    /// Decorator instance
    GtpyDecorator* m_decorator;

    /// Currently used context
    GtpyContextManager::Context m_currentContex;

    /// Determines whether output messages are sent to console.
    bool m_sendOutputMessages;

    /// Determines whether error messages are sent to console.
    bool m_sendErrorMessages;

    /// True if error message was sent
    bool m_errorEmitted;

    /// Lists of added objects for all contexts
    QMap<int, QStringList> m_addedObjectNames;

    /// Calculator accessible contexts
    QList<GtpyContextManager::Context> m_calcAccessibleContexts;

private slots:
    /**
     * @brief Emits errorCodeLine signal.
     * @param message Error message.
     */
    void onErrorLine(const QString& message);

    /**
     * @brief Emits errorMessage signal.
     * @param message Error message.
     */
    void onErrorMessage(const QString& message);

    /**
     * @brief Emits pythonMessage signal.
     * @param message Message.
     */
    void onPythonMessage(const QString& message);

    /**
     * @brief Keeps GTlab running when a python script calls sys.exit().
     * @param exep Exception.
     */
    void onSystemExitExceptionRaised(int exep);

signals:
    /**
     * @brief Sends the line number in which an error was detected.
     * @param line Line number in which an error was detected.
     * @param type Python context in which the error occurred.
     */
    void errorCodeLine(int line, GtpyContextManager::Context type);

    /**
     * @brief Sends an error messages.
     * @param message Error message.
     * @param type Python context in which the error occurred.
     */
    void errorMessage(const QString& message,
                      const GtpyContextManager::Context& type);

    /**
     * @brief Sends python messages.
     * @param message Pyhton message.
     * @param type Python context in which the message was occurred.
     */
    void pythonMessage(const QString& message,
                       const GtpyContextManager::Context& type);

    /**
     * @brief Emitted if script evaluation starts.
     * @param type Python context in which the evaluation starts.
     * @param outputToEveryConsole True, if output is to be sent to all
     * consoles.
     */
    void startedScriptEvaluation(const GtpyContextManager::Context& type,
                                 bool outputToEveryConsole);

    /**
     * @brief Emitted after script evaluation.
     * @param type Python context in which script was evaluated.
     */
    void scriptEvaluated(const GtpyContextManager::Context& type);

};

Q_DECLARE_METATYPE(GtpyContextManager::Context);

/**
 * @brief The GtpyTypeConversion class
 */
class GtpyTypeConversion
{
public:
    /**
     * @brief Converts QMap<int, double> instances to a Python object.
     * @param inObject QMap instance.
     * @return Given QMap instance as Python object.
     */
    static PyObject* convertFromQMapIntDouble(const void* inObject,
                                                       int /*metaTypeId*/);

    static bool convertToQMapIntDouble(PyObject* obj, void* outMap,
                                       int /*metaTypeId*/, bool /*strict*/);

    static PyObject* convertFromQMapStringDouble(const void* inObject,
                                                       int /*metaTypeId*/);

    static bool convertToQMapStringDouble(PyObject* obj, void* outMap,
                                       int /*metaTypeId*/, bool /*strict*/);

    static PyObject* convertFromQMapStringInt(const void* inObject,
                                                       int /*metaTypeId*/);

    static bool convertToQMapStringInt(PyObject* obj, void* outMap,
                                       int /*metaTypeId*/, bool /*strict*/);

private:
    /**
     * @brief Template function for converting QMap instances into Python
     * objects independent of the key/value type.
     * @param map Pointer to map instance.
     * @return Given map instance as Python object.
     */

    template<typename Key, typename Val>
    static PyObject* mapToPython (const void* map);

    /**
     * @brief pythonToMap
     * @param val
     * @param result
     */
    template <typename Key, typename Val>
    static bool pythonToMap(PyObject* obj, void* outMap);
};

#endif // GTPY_CONTEXTMANAGER_H
