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
#include <QMutex>

#include "PythonQtObjectPtr.h"
#include "PythonQtConversion.h"

#include "gtpy_gilscope.h"
#include "gtpy_globals.h"

class GtTask;
class GtObject;
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
        BatchContext = 0,
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

    struct PythonContext
    {
        PythonQtObjectPtr module = Q_NULLPTR;
        QString contextName;
    };

    /**
    * @brief Returns instance of context manager based on singleton pattern.
    * @return Instance of context manager
    */
    static GtpyContextManager* instance();

    ~GtpyContextManager();

    void initExtensions();

    /**
    * @brief Evaluates the given script into the given python context.
    * @param contextId Python context identifier.
    * @param script The script to be executed.
    * @param output Emit output messages.
    * @param errorMessage Emit error messages.
    * @param option Evaluation options.
    * @return True if evaluation was successful.
    */
    bool evalScript(int contextId, const QString& script,
                    const bool output = true, const bool errorMessage = true,
                    const GtpyContextManager::EvalOptions& option = EvalFile);

    /**
    * @brief introspection
    * @param contextId
    * @param objectname
    * @param appendModules
    * @return
    */
    QMultiMap<QString, GtpyFunction> introspection(int contextId,
            const QString& objectname,
            const bool appendModules = false);

    /**
    * @brief Adds the object obj to the Python context indicated by
    *  contextId. After calling this function, the object can be referenced by
    *  the specified name.
    * @param contextId Python context identifier.
    * @param name Reference name in Python context.
    * @param obj Object to add.
    * @param saveName If true, the reference name is stored in the list of
    * added objects. All stored objects will be removed by calling the
    * removeAllAddedObjectes() function.
    * @return Whether the addition was successful.
    */
    bool addObject(int contextId,
                   const QString& name, QObject* obj,
                   const bool saveName = true);

    /**
    * @brief Adds the object obj to the Python context indicated by
    *  contextId. After calling this function, the object can be referenced by
    *  the specified name. This function wraps the object with the
    *  GtpyExtendedWrapper class.
    * @param contextId Python context identifier.
    * @param name Reference name in Python context.
    * @param obj Object to add.
    * @param saveName If true, the reference name is stored in the list of
    * added objects. All stored objects will be removed by calling the
    * removeAllAddedObjectes() function.
    * @return Whether the addition was successful.
    */
    bool addGtObject(int contextId,
                     const QString& name, QObject* obj,
                     const bool saveName = true);

    /**
    * @brief Removes the object with the given name from the Python context
    * indicated by contextId.
    * @param contextId Python context identifier.
    * @param name Reference name in Python context.
    * @return Whether the removal was successful.
    */
    bool removeObject(int contextId, const QString& name);

    /**
    * @brief Removes all objects added by calling the addObject() function from
    * the Python context indicated by contextId. Note that it only removes the
    * objctes whose reference name is stored in the list of added objects.
    * @param contextId Python context identifier.
    * @return Whether the removal was successful.
    */
    bool removeAllAddedObjects(int contextId);

    /**
    * @brief Adds the task object to the Python context indicated by
    *  contextId.
    * @param contextId Python context identifier.
    * @param task Task object to add.
    * @return Whether the addition was successful.
    */
    bool addTaskValue(int contextId, GtTask* task);

    //    bool addDataModelObject(int contextId, const QString& name,
    //                            GtObject* package);

    /**
    * @brief Checks whether the Python context indicated by contextId as access
    *  to calculators. If the context has access, this function deletes all
    * calculator instances that are children of the task instance (TASK_VAR).
    * @param contextId Python context identifier.
    */
    void deleteCalcsFromTask(int contextId);

    /**
    * @brief Converts QVariant value val to QString value.
    * @param val Value to convert.
    * @return QString value.
    */
    QString qvariantToPyStr(const QVariant& val) const;

    /**
    * @brief Returns the name of the find child function defined in decorator
    *  class.
    * @return Name of find child function as QString.
    */
    QString findChildFuncName() const;

    /**
    * @brief Returns the name of the set property value function defined in
    * decorator class.
    * @return Name of set property value function as QString.
    */
    QString setPropertyValueFuncName() const;

    /**
    * @brief Initializes a Python context for each value of the enum Context.
    */
    void initContexts();

    /**
    * @brief Creates a new python context and returns its id.
    * @param type Determines the type of context and thus which
    * functionalities it has.
    * @param emitSignal If true, newContextCreated(contextId) will be emitted.
    * @return The id of the new created context.
    */
    int createNewContext(const GtpyContextManager::Context& type,
                         bool emitSignal = false);

    /**
    * @brief Deletes the python context with the given id.
    * @param contextId Id of a python context.
    * @param emitSignal If true, contextDeleted(contextId) will be emitted.
    * @return True, if the deletion was successful.
    */
    bool deleteContext(int contextId, bool emitSignal = false);

    /**
    * @brief Resets the Python context indicated by contextId to and
    * initializes it with the functionality of type.
    * @param type Identifies the functionality of the context after the reset.
    * @param contextId Id of a python context.
    */
    void resetContext(const GtpyContextManager::Context& type,
                      int contextId = -1);

    /**
    * @brief Returns the id of the current Python thread.
    * @return Id of the current Python thread.
    */
    long currentPyThreadId();

    /**
    * @brief Starts a runnable instance which interrupt the current Python
    * thread.
    * @param id Id of the Python thread to interrupt.
    */
    void interruptPyThread(long id);

    /**
     * @brief Retruns the meta data saved in thread dict.
     * @return The meta data saved in thread dict.
     */
    GtpyGlobals::StdOutMetaData threadDictMetaData();

    /**
     * @brief Sets some meta data to the thread dict.
     * @param metaData
     */
    void setMetaDataToThreadDict(GtpyGlobals::StdOutMetaData metaData);

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
    static const QString CLASS_WRAPPER_MODULE;
    static const QString CALC_MODULE;

    /**
    * @brief Configures the python context indicated by contextId with the
    * functionality indicated by type
    * @param type Identifies the functionality of the context.
    * @param contextId Id of a python context.
    * @param contextName The name of the Python context.
    */
    void defaultContextConfig(const GtpyContextManager::Context& type,
                              int contextId,
                              const QString& contextName);

    /**
    * @brief Calls the function containing the specific configuration for the
    * Python context indicated by contextId. The type determines the
    * configuration of the context.
    * @param type Python context identifier.
    * @param contextId Id of a python context.
    */
    void specificContextConfig(const GtpyContextManager::Context& type,
                               int contextId);

    /**
    * @brief Returns a pointer to the Python context indicated by contextId.
    * @param contextId Python context identifier.
    * @return Pointer to Python Context.
    */
    PythonQtObjectPtr context(int contextId) const;

    /**
    * @brief Initializes the calculator module.
    */
    void initCalculatorsModule();

    /**
    * @brief Initializes the logging module.
    */
    void initLoggingModule();

    /**
    * @brief Initializes the import behaviour of python.
    */
    void initImportBehaviour();

    /**
    * @brief Initializes additional functionalities for the context of type
    * BatchContext.
    * @param contextId Python context identifier.
    */
    void initBatchContext(int contextId);

    /**
    * @brief Initializes additional functionalities for the context of type
    * GlobalContext.
    * @param contextId Python context identifier.
    */
    void initGlobalContext(int contextId);

    /**
    * @brief Initializes additional functionalities for context of type
    * ScriptEditorContext.
    * @param contextId Python context identifier.
    */
    void initScriptEditorContext(int contextId);

    /**
    * @brief Initializes additional functionalities for context of type
    * CalculatorRunContext.
    * @param contextId Python context identifier.
    */
    void initCalculatorRunContext(int contextId);

    /**
    * @brief Initializes additional functionalities for context of type
    * TaskEditorContext.
    * @param contextId Python context identifier.
    */
    void initTaskEditorContext(int contextId);

    /**
    * @brief Initializes additional functionalities for context of type
    * TaskRunContex.
    * @param contextId Python context identifier.
    */
    void initTaskRunContext(int contextId);

    /**
    * @brief Initializes the output behaviour of python.
    */
    void initStdOut();

    void initWrapperModule();

    /**
    * @brief Imports the default modules to the Python context identified by
    * contextId.
    * @param contextId Python context identifier.
    */
    void importDefaultModules(int contextId);

    /**
    * @brief Imports the logging functions to the context identified by
    * contextId. Stores the appConsole value that specifies whether the
    * logging functions send the output to the application console.
    * @param contextId Python context identifier.
    * @param appConsole If true, the logging messages are sent to the
    * GTlab application console.
    */
    void importLoggingFuncs(int contextId, bool appConsole);

    /**
    * @brief Imports the calculator module to the context identified by
    * contextId.
    * @param contextId Python context identifier.
    */
    void importCalcModule(int contextId);

    /**
    * @brief Searches the line number out of an error message.
    * @param message The error message.
    * @return The line number in which an error was detected.
    */
    static int lineOutOfMessage(const QString& message);

    /**
    * @brief Sets some meta data to the thread dict.
    * @param contextId Python context identifier.
    * @param output Send output messages.
    * @param error Send error messages.
    */
    void setMetaDataToThreadDict(int contextId, bool output, bool error);

    /**
    * @brief introspectObject
    * @param object
    * @return
    */
    QMultiMap<QString, GtpyFunction> introspectObject(PyObject* object) const;

    /**
    * @brief Used to define extra functions or keywords for the completer
    * @return custom completions
    */
    QMultiMap<QString, GtpyFunction> customCompletions() const;

    /**
    * @brief Used to obtain the built-in functions of python
    * @return builtin fucntions of python
    */
    QMultiMap<QString, GtpyFunction> builtInCompletions(int contextId) const;

    /**
    * @brief Obtains importable modules and sets it to member variable
    */
    void setImportableModulesCompletions(int contextId);

    /**
    * @brief Returns the constractor functions for the calculators.
    * @param contextId Python context identifier.
    * @return Constructor functions for the calculators.
    */
    QMultiMap<QString, GtpyFunction> calculatorCompletions(int contextId) const;

    /**
    * @brief Sets custom completions and builtin completions to member
    *  variable. Removes duplicates in these
    * @param contextId Python context identifier.
    */
    void setStandardCompletions(int contextId);

    /**
    * @brief Registers the type converters in PythonQt that implemented in
    * the GtpyTypeConversion class.
    */
    void registerTypeConverters() const;

    /**
    * @brief Used to obtain the Python version currently active
    * @return Major and minorversion of current Python instance (e.g '3.7')
    */
    QString pythonVersion() const;

    /**
    * @brief Callback for stdout redirection.
    * It emits the pythonMessage() signal to share the message.
    * @param contextName Name of the context in which the message was
    * triggered.
    * @param output Determines whether the output should be displayed or not.
    * @param error Determines whether the error should be displayed or not.
    * @param message Message to emit.
    */
    static void stdOutRedirectCB(const QString& contextName, const bool output,
                                 const bool error, const QString& message);

    /**
    * @brief Callback for stderr redirection.
    * It emits the errorMessage() and the errorCodeLine() signal to share the
    * error message.
    * @param contextName Name of the context in which the error occurred.
    * @param output Determines whether the output should be displayed or not.
    * @param error Determines whether the error should be displayed or not.
    * @param message Error message to emit.
    */
    static void stdErrRedirectCB(const QString& contextName, const bool output,
                                 const bool error, const QString& message);

    /**
    * @brief Returns the id of the context with the given name.
    * @param contextName Name of the context.
    * @return The id of the context with the given name.
    */
    int contextIdByName(const QString& contextName);

    /**
    * @brief Returns the name of the context with the given id.
    * @param contextId Id of the context.
    * @return The name of the context with the given id.
    */
    QString contextNameById(int contextId);

    /// Map of Python context
    QMap<int, PythonContext> m_contextMap;

    /// Whether the contexts send messages to the application console
    QMap<int, bool> m_appLogging;

    /// Logging Module
    PythonQtObjectPtr m_loggingModule;

    /// Error message emitter
    PythonQtObjectPtr m_err;

    /// Output emitter
    PythonQtObjectPtr m_out;

    /// Standard completions for pytho completer
    QMultiMap<QString, GtpyFunction> m_standardCompletions;

    /// Contains completions of importable modules
    QMultiMap<QString, GtpyFunction> m_importableModulesCompletions;

    /// Decorator instance
    GtpyDecorator* m_decorator;

    /// Determines whether output messages are sent to console.
    bool m_sendOutputMessages;

    /// Determines whether error messages are sent to console.
    bool m_sendErrorMessages;

    /// True if error message was sent
    bool m_errorEmitted;

    /// Lists of added objects for all contexts
    QMap<int, QStringList> m_addedObjectNames;

    /// Calculator accessible contexts
    QList<int> m_calcAccessibleContexts;

    /// Python main thread state
    PyThreadState* m_pyThreadState;

    bool m_contextsInitialized;

    QMutex m_evalMutex;

private slots:
    /**
    * @brief Emits errorMessage signal.
    * @param message Error message.
    */
    void onErrorMessage(const QString& message);

    /**
    * @brief Keeps GTlab running when a python script calls sys.exit().
    * @param exep Exception.
    */
    void onSystemExitExceptionRaised(const int exep) const;

signals:
    /**
    * @brief Sends the line number in which an error was detected.
    * @param line Line number in which an error was detected.
    * @param contextId Python context in which the error occurred.
    */
    void errorCodeLine(int line, int contextId);

    /**
    * @brief Sends an error messages.
    * @param message Error message.
    * @param contextId Python context in which the error occurred.
    */
    void errorMessage(const QString& message, int contextId);

    /**
    * @brief Sends python messages.
    * @param message Python message.
    * @param contextId Python context in which the message was occurred.
    */
    void pythonMessage(const QString& message, int contextId);

    /**
    * @brief Emitted if script evaluation starts.
    * @param contextId Python context in which the evaluation starts.
    */
    void startedScriptEvaluation(int contextId);

    /**
    * @brief Emitted after script evaluation.
    * @param contextId Python context in which script was evaluated.
    */
    void scriptEvaluated(int contextId);

    /**
    * @brief Emitted after creating a new context.
    * @param contextId Context id.
    */
    void newContextCreated(int contextId);

    /**
    * @brief Emitted after a deleted.
    * @param contextId Cotnext Id.
    */
    void contextDeleted(int contextId);

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
            const int /*metaTypeId*/);

    /**
    * @brief Converts Python object to QMap<int, double>.
    * @param obj Python object.
    * @param outMap QMap output.
    * @return Whether the conversion was successful or not.
    */
    static bool convertToQMapIntDouble(PyObject* obj, void* outMap,
                                       int /*metaTypeId*/, bool /*strict*/);

    /**
    * @brief Converts QMap<QString, double> instances to a Python object.
    * @param inObject QMap instance.
    * @return Given QMap instance as Python object.
    */
    static PyObject* convertFromQMapStringDouble(const void* inObject,
            int /*metaTypeId*/);
    /**
    * @brief Converts Python object to QMap<QString, double>.
    * @param obj Python object.
    * @param outMap QMap output.
    * @return Whether the conversion was successful or not.
    */
    static bool convertToQMapStringDouble(PyObject* obj, void* outMap,
                                          int /*metaTypeId*/, bool /*strict*/);
    /**
    * @brief Converts QMap<QString, int> instances to a Python object.
    * @param inObject QMap instance.
    * @return Given QMap instance as Python object.
    */
    static PyObject* convertFromQMapStringInt(const void* inObject,
            int /*metaTypeId*/);
    /**
    * @brief Converts Python object to QMap<QString, int>.
    * @param obj Python object.
    * @param outMap QMap output.
    * @return Whether the conversion was successful or not.
    */
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
    static PyObject* mapToPython(const void* inMap)
    {
        GTPY_GIL_SCOPE

        QMap<Key, Val>& map = *((QMap<Key, Val>*) inMap);

        PyObject* result = PyDict_New();

        //QMap<Key, Val>::const_iterator    t;
        //t = map.constBegin();
        QList<Key> keys = map.keys();

        PyObject* key = Q_NULLPTR;
        PyObject* val = Q_NULLPTR;

        //for ( ; t != map.constEnd(); t++)
        foreach (Key k, keys)
        {
            // converts key and value to QVariant and then to PyObject*
            key = PythonQtConv::QVariantToPyObject(QVariant(k));
            val = PythonQtConv::QVariantToPyObject(QVariant(map.value(k)));

            // sets key and val to the result dict
            PyDict_SetItem(result, key, val);

            // decrement the reference count for key and val
            Py_DECREF(key);
            Py_DECREF(val);
        }

        return result;
    }

    /**
    * @brief Template function for converting Python objects into QMap
    * instances independent of the key/value type.
    * @param obj Python object to convert.
    * @param outMap QMap output.
    * @return Whether the conversion was successful or not.
    */
    template <typename Key, typename Val>
    static bool pythonToMap(PyObject* obj, void* outMap)
    {
        GTPY_GIL_SCOPE

        bool success = false;

        if (PyMapping_Check(obj))
        {
            QMap<Key, Val>& map = *((QMap<Key, Val>*) outMap);

            QString tempFunc = "items";
            QByteArray ba = tempFunc.toLocal8Bit();
            char* func = ba.data();

            PyObject* items = PyObject_CallMethod(obj, func, NULL);

            if (items)
            {
                int count = PyList_Size(items);

                PyObject* pyValue = Q_NULLPTR;
                PyObject* pyKey = Q_NULLPTR;
                PyObject* pyTuple = Q_NULLPTR;

                for (int i = 0; i < count; i++)
                {
                    pyTuple = PyList_GetItem(items, i);
                    pyKey = PyTuple_GetItem(pyTuple, 0);
                    pyValue = PyTuple_GetItem(pyTuple, 1);

                    Py_XINCREF(pyTuple);
                    Py_XINCREF(pyKey);
                    Py_XINCREF(pyKey);

                    QVariant key = PythonQtConv::PyObjToQVariant(pyKey);
                    QVariant val = PythonQtConv::PyObjToQVariant(pyValue);

                    map.insert(key.value<Key>(), val.value<Val>());

                    Py_XDECREF(pyTuple);
                    Py_XDECREF(pyKey);
                    Py_XDECREF(pyKey);
                }

                Py_DECREF(items);

                success = true;
            }
        }

        return success;
    }

};

#endif // GTPY_CONTEXTMANAGER_H
