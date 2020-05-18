/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_contextmanager.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaEnum>
#include <QSettings>
#include <QThreadPool>
#include <QRegularExpression>

#include "PythonQt.h"
#include "PythonQtObjectPtr.h"
#include "PythonQtMethodInfo.h"
#include "PythonQtSignal.h"
#include "PythonQtProperty.h"
#include "PythonQtConversion.h"

#include "gt_logging.h"
#include "gt_abstractproperty.h"
#include "gt_project.h"
#include "gt_processdata.h"
#include "gt_task.h"
#include "gt_calculator.h"
#include "gt_coreapplication.h"
#include "gt_calculatordata.h"
#include "gt_calculatorfactory.h"
#include "gt_datazone0d.h"
#include "gt_calculatorhelperfactory.h"

#include "gtpy_globals.h"
#include "gtpy_calculatorfactory.h"
#include "gtpy_decorator.h"
#include "gtpy_interruptrunnable.h"
#include "gtpy_stdout.h"

#include "gtpy_extendedwrapper.h"
#include "gtpy_createhelperfunction.h"
#include "gtpy_propertysetter.h"
#include "gtpy_importfunction.h"
#include "gtpy_taskfindermodule.h"

#include "gtpy_contextmanager.h"

///critical static strings
const QString GtpyContextManager::LIST_DATATYPE = QStringLiteral("QList");
const QString GtpyContextManager::VARIANT_DATATYPE =
    QStringLiteral("QVariant");

const QString GtpyContextManager::FUNCTION_WARNING =
    QStringLiteral("WARNING: function not found");

const QString GtpyContextManager::TASK_VAR =
    QStringLiteral("__task");
const QString GtpyContextManager::CALC_FAC_VAR =
    QStringLiteral("__calculatorFactory");
const QString GtpyContextManager::HELPER_FAC_VAR =
    QStringLiteral("__helperFactory");
const QString GtpyContextManager::GTOBJECT_WRAPPER =
    QStringLiteral("CurrentGtObjectWrapper");
const QString GtpyContextManager::HELPER_WRAPPER =
    QStringLiteral("HelperWrapper");

const QString GtpyContextManager::DEFAULT_IMPORT =
    QStringLiteral("default_imp");

const QString GtpyContextManager::CLASS_WRAPPER_MODULE =
    QStringLiteral("GtClasses");
const QString GtpyContextManager::LOGGING_MODULE =
    QStringLiteral("GtLogging");
const QString GtpyContextManager::CALC_MODULE =
    QStringLiteral("GtCalculators");
const QString GtpyContextManager::DEFAULT_IMPORT_MODULE =
    QStringLiteral("__GtDefaultImport");
const QString GtpyContextManager::CUSTOM_IMPORT_MODULE =
    QStringLiteral("__GtCustomImport");
const QString GtpyContextManager::OBJECT_WRAPPER_MODULE =
    QStringLiteral("GtObjectWrapperModule");

GtpyContextManager::GtpyContextManager(QObject* parent) :
    QObject(parent), m_decorator(Q_NULLPTR),
    m_errorEmitted(false),
    m_loggingModule(Q_NULLPTR),
    m_pyThreadState(Q_NULLPTR),
    m_contextsInitialized(false)
{
    qRegisterMetaType<GtpyContextManager::Context>
    ("GtpyContextManager::Context");

    PythonQt::init(PythonQt::RedirectStdOut);

    GtpyGilScope::setGILScopeEnabled(true);

    PythonQt::self()->setSystemExitExceptionHandlerEnabled(true);

    PythonQt::self()->registerClass(&GtProject::staticMetaObject,
                                    CLASS_WRAPPER_MODULE.toLocal8Bit().data());
    PythonQt::self()->registerClass(&GtAbstractProperty::staticMetaObject,
                                    CLASS_WRAPPER_MODULE.toLocal8Bit().data());
    PythonQt::self()->registerClass(&GtObject::staticMetaObject,
                                    CLASS_WRAPPER_MODULE.toLocal8Bit().data());
    PythonQt::self()->registerClass(&GtTask::staticMetaObject,
                                    CLASS_WRAPPER_MODULE.toLocal8Bit().data());
    PythonQt::self()->registerClass(&GtPythonLogger::staticMetaObject,
                                    CLASS_WRAPPER_MODULE.toLocal8Bit().data());
    PythonQt::self()->registerClass(&GtCalculator::staticMetaObject,
                                    CLASS_WRAPPER_MODULE.toLocal8Bit().data());
    PythonQt::self()->registerClass(&GtDataZone0D::staticMetaObject,
                                    CLASS_WRAPPER_MODULE.toLocal8Bit().data());

    registerTypeConverters();

    m_decorator = new GtpyDecorator(this);

    PythonQt::self()->addDecorators(m_decorator);

    PythonQt::self()->registerCPPClass("GtpyProcessDataDistributor", 0,
                                     CLASS_WRAPPER_MODULE.toLocal8Bit().data());

    connect(m_decorator, SIGNAL(sendErrorMessage(QString)), this,
            SLOT(onErrorMessage(QString)));

    connect(PythonQt::self(), SIGNAL(systemExitExceptionRaised(int)), this,
            SLOT(onSystemExitExceptionRaised(int)));

    if (PyType_Ready(&GtpyStdOutRedirect_Type) < 0)
    {
        gtError() << "could not initialize GtpyStdOutRedirect_Type";
    }
    Py_INCREF(&GtpyStdOutRedirect_Type);

    if (PyType_Ready(&GtpyExtendedWrapper_Type) < 0)
    {
        gtError() << "could not initialize GtpyExtendedWrapper_Type";
    }
    Py_INCREF(&GtpyExtendedWrapper_Type);

    if (PyType_Ready(&GtpyCreateHelperFunction_Type) < 0)
    {
        gtError() << "could not initialize GtpyCreateHelperFunction_Type";
    }
    Py_INCREF(&GtpyCreateHelperFunction_Type);

    if (PyType_Ready(&GtpyPropertySetter_Type) < 0)
    {
        gtError() << "could not initialize GtpyPropertySetter_Type";
    }
    Py_INCREF(&GtpyPropertySetter_Type);

    if (PyType_Ready(&GtpyMyImport_Type) < 0)
    {
        gtError() << "could not initialize GtpyMyImport_Type";
    }
    Py_INCREF(&GtpyMyImport_Type);

    m_pyThreadState = PyEval_SaveThread();

    GtpyCustomization::customizeSlotCalling();
}

GtpyContextManager*
GtpyContextManager::instance()
{
    static GtpyContextManager* retval = Q_NULLPTR;

    if (retval == Q_NULLPTR)
    {
        retval = new GtpyContextManager(gtApp);
    }

    return retval;
}

GtpyContextManager::~GtpyContextManager()
{
    if (m_pyThreadState != Q_NULLPTR)
    {
        PyEval_RestoreThread(m_pyThreadState);
    }
}

bool
GtpyContextManager::evalScript(int contextId,
                               const QString& script,
                               const bool output,
                               const bool errorMessage,
                               const EvalOptions& option)
{
    if (!m_contextsInitialized)
    {
        initContexts();
    }

    GTPY_GIL_SCOPE

    setMetaDataToThreadDict(contextId, output, errorMessage);

    if (output)
    {
        emit startedScriptEvaluation(contextId);
    }

    PythonQtObjectPtr currentContext = context(contextId);

    if (currentContext == Q_NULLPTR)
    {
        return false;
    }

    if (m_appLogging.value(contextId, false))
    {
        m_loggingModule.evalScript(QStringLiteral(
                                 "PyLogger._PyLogger__outputToConsole = True"));
    }
    else
    {
        m_loggingModule.evalScript(QStringLiteral(
                                "PyLogger._PyLogger__outputToConsole = False"));
    }

//    if (m_calcAccessibleContexts.contains(contextId))
//    {
//        QString pyCode =
//                "if '" + CALC_MODULE + "' in locals():\n" +
//                "    import sys\n" +
//                "    sys.modules['" + CALC_MODULE + "'] = " + CALC_MODULE +
//                "\n" +
//                "    del sys\n";

//        currentContext.evalScript(pyCode);
//    }

    bool hadError = false;

    if (!script.isEmpty())
    {
        currentContext.evalScript(script, option);

        m_evalMutex.lock();
        hadError = PythonQt::self()->hadError();
        PythonQt::self()->clearError();
        m_evalMutex.unlock();
    }

    if (output || (hadError && errorMessage))
    {
        emit scriptEvaluated(contextId);
    }

//    if (m_calcAccessibleContexts.contains(contextId))
//    {
//        removeCalcModuleFromSys();
//    }

    return !hadError;
}

QMultiMap<QString, GtpyFunction>
GtpyContextManager::introspection(int contextId, const QString& objectname,
                                  const bool appendModules)
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr currentContext = context(contextId);

    if (currentContext == Q_NULLPTR)
    {
        return QMultiMap<QString, GtpyFunction>();
    }

    QMultiMap<QString, GtpyFunction> results;

    PythonQtObjectPtr object;

    const bool objNameIsEmpty = objectname.isEmpty();

    static bool standardCompletionsSet = false;

    if (!standardCompletionsSet)
    {
        setStandardCompletions(contextId);
        setImportableModulesCompletions(contextId);
        standardCompletionsSet = true;
    }

    if (objNameIsEmpty)
    {
        object = currentContext;
    }
    else
    {
        object = PythonQt::self()->lookupObject(currentContext,
                                                objectname);
    }

    if (object)
    {
        results = introspectObject(object);
    }
    else
    {
        QString temp = QStringLiteral("obj");
        QString script = temp + QStringLiteral(" = ") + objectname;

        bool eval = evalScript(contextId, script, false, false, EvalSingleString);

        if (eval)
        {
            object = PythonQt::self()->lookupObject(currentContext,
                                                    temp);

            if (object)
            {
                results = introspectObject(object);
            }

            currentContext.removeVariable(temp);
        }
    }

    if (objNameIsEmpty)
    {
        results = results + calculatorCompletions(contextId) +
                  m_standardCompletions;

        if (appendModules)
        {
            results += m_importableModulesCompletions;
        }
    }

    return results;
}

bool
GtpyContextManager::addObject(int contextId,
                              const QString& name, QObject* obj,
                              const bool saveName)
{
    if (name.isEmpty())
    {
        return false;
    }

    if (obj == Q_NULLPTR)
    {
        return false;
    }

    PythonQtObjectPtr currentContext = context(contextId);

    if (currentContext == Q_NULLPTR)
    {
        return false;
    }

    QStringList list = m_addedObjectNames.value(contextId, QStringList());

    if (list.contains(name))
    {
        return false;
    }

    if (saveName)
    {
        list.append(name);
        m_addedObjectNames.insert(contextId, list);
    }

    GTPY_GIL_SCOPE

    currentContext.addObject(name, obj);

    return true;
}

bool
GtpyContextManager::addGtObject(int contextId, const QString& name,
                                QObject* obj, const bool saveName)
{
    if (name.isEmpty())
    {
        return false;
    }

    if (obj == Q_NULLPTR)
    {
        return false;
    }

    PythonQtObjectPtr currentContext = context(contextId);

    if (currentContext == Q_NULLPTR)
    {
        return false;
    }

    QStringList list = m_addedObjectNames.value(contextId, QStringList());

    if (list.contains(name))
    {
        return false;
    }

    if (saveName)
    {
        list.append(name);
        m_addedObjectNames.insert(contextId, list);
    }

    GTPY_GIL_SCOPE

    PyObject* argsTuple = PyTuple_New(1);

    PyObject* pyQtWrapper = PythonQt::priv()->wrapQObject(obj);

    if (pyQtWrapper && (pyQtWrapper->ob_type->tp_base ==
                            &PythonQtInstanceWrapper_Type))
    {
        PyTuple_SetItem(argsTuple, 0, pyQtWrapper);
    }
    else
    {
        Py_XDECREF(pyQtWrapper);
        Py_DECREF(argsTuple);
        return false;
    }

    PyObject* wrapped = GtpyExtendedWrapper_Type.tp_new(
                            &GtpyExtendedWrapper_Type, argsTuple, NULL);

    Py_DECREF(argsTuple);

    PyModule_AddObject(currentContext, name.toLatin1().data(), wrapped);

    return true;
}

bool
GtpyContextManager::removeObject(int contextId, const QString& name)
{
    GTPY_GIL_SCOPE

    QStringList list = m_addedObjectNames.value(contextId, QStringList());

    if (!list.contains(name))
    {
        return false;
    }

    PythonQtObjectPtr currentContext = context(contextId);

    if (currentContext == Q_NULLPTR)
    {
        return false;
    }

    currentContext.removeVariable(name);

    list.removeOne(name);

    m_addedObjectNames.insert(contextId, list);

    return true;
}

bool
GtpyContextManager::removeAllAddedObjects(int contextId)
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr currentContext = context(contextId);

    if (currentContext == Q_NULLPTR)
    {
        return false;
    }

    QStringList list = m_addedObjectNames.value(contextId, QStringList());

    foreach (QString objName, list)
    {
        currentContext.removeVariable(objName);
    }

    m_addedObjectNames.insert(contextId, QStringList());

    return true;
}

bool
GtpyContextManager::addTaskValue(int contextId, GtTask* task)
{
    GTPY_GIL_SCOPE

    if (!m_calcAccessibleContexts.contains(contextId))
    {
        return false;
    }

    if (task != Q_NULLPTR)
    {
        addObject(contextId, TASK_VAR, task, false);

        QString pyCode = "if '" + CALC_MODULE + "' in globals():\n" +
                         "    " + CALC_MODULE + "." + TASK_VAR + " = "
                         + TASK_VAR;

        evalScript(contextId, pyCode , false);
    }
    else
    {
        QString pyCode = TASK_VAR + " = None"
                         "if '" + CALC_MODULE + "' in globals():\n" +
                         "    " + CALC_MODULE + "." + TASK_VAR + " = None";

        evalScript(contextId, pyCode, false);
    }

    return true;
}

//bool
//GtpyContextManager::addDataModelObject(int contextId, const QString& name,
//                                       GtObject* package)
//{
//    GTPY_GIL_SCOPE

//    if (package != Q_NULLPTR && !name.isEmpty())
//    {
//        addObject(contextId, name, package);

//        QString pyCode = "if '" + GTOBJECT_WRAPPER + "' in globals():\n" +
//                         "    globals()['"+ name + "'] = " + GTOBJECT_WRAPPER +
//                         "(" + name + ")\n";

//        evalScript(contextId, pyCode , false);

//        return true;
//    }

//    return false;
//}

void
GtpyContextManager::deleteCalcsFromTask(int contextId)
{
    if (!m_calcAccessibleContexts.contains(contextId))
    {
        return;
    }

    evalScript(contextId, CALC_MODULE + QStringLiteral(".") + TASK_VAR
               + QStringLiteral(".deleteAllCalculators()"), false);
}

QString
GtpyContextManager::qvariantToPyStr(const QVariant& val) const
{
    GTPY_GIL_SCOPE

    PyObject* pyObj = PythonQtConv::QVariantToPyObject(val);

    QString retVal = PythonQtConv::PyObjGetString(pyObj);

    Py_XDECREF(pyObj);

    return retVal;
}

QString
GtpyContextManager::findChildFuncName() const
{
    return m_decorator->getFunctionName(FIND_GT_CHILD_TAG);
}

QString
GtpyContextManager::setPropertyValueFuncName() const
{
    return m_decorator->getFunctionName(SET_PROPERTY_VALUE_TAG);
}

void
GtpyContextManager::initContexts()
{
    initLoggingModule();
    initWrapperModule();
    initImportBehaviour();
    initTaskFinderModule();

    QMetaObject metaObj = GtpyContextManager::staticMetaObject;
    QMetaEnum metaEnum = metaObj.enumerator(
                             metaObj.indexOfEnumerator("Context"));

    int keyCount = metaEnum.keyCount();

    for (int i = 0;  i < keyCount; i++)
    {
        QString contextName = QString::fromUtf8(metaEnum.key(i));

        Context type = static_cast<Context>(metaEnum.value(i));

        int contextId = metaEnum.value(i);

        defaultContextConfig(type, contextId, contextName);
    }

    initStdOut();

    m_contextsInitialized = true;
}

int
GtpyContextManager::createNewContext(const GtpyContextManager::Context& type,
                                     bool emitSignal)
{
    QMetaObject metaObj = GtpyContextManager::staticMetaObject;
    QMetaEnum metaEnum = metaObj.enumerator(
                             metaObj.indexOfEnumerator("Context"));

    QString contextName = QString::fromUtf8(metaEnum.key((int)type));

    if (contextName.isEmpty())
    {
        return -1;
    }

    QList<int> keys = m_contextMap.keys();

    if (keys.isEmpty())
    {
        return -1;
    }

    int contextId = *std::max_element(keys.begin(), keys.end()) + 1;

    contextName += ("_" + QString::number(contextId));

    defaultContextConfig(type, contextId, contextName);

    if (emitSignal)
    {
        emit newContextCreated(contextId);
    }

    return contextId;
}

bool
GtpyContextManager::deleteContext(int contextId, bool emitSignal)
{
    QMetaObject metaObj = GtpyContextManager::staticMetaObject;
    QMetaEnum metaEnum = metaObj.enumerator(
                             metaObj.indexOfEnumerator("Context"));

    if (contextId < metaEnum.keyCount())
    {
        gtDebug() << "It is not allowed to remove one of the default contexts!";
        return false;
    }

    PythonContext con = m_contextMap.take(contextId);

    if (!con.module)
    {
        return false;
    }

    GTPY_GIL_SCOPE

    PythonQtObjectPtr sys;
    sys.setNewRef(PyImport_ImportModule("sys"));

    PyObject* dict = PyModule_GetDict(sys);

    if (!dict)
    {
        return false;
    }

    Py_INCREF(dict);

    PyObject* modules = PyDict_GetItemString(dict, "modules");

    if (!modules)
    {
        Py_DECREF(dict);
        return false;
    }

    Py_INCREF(modules);

    PyDict_DelItemString(modules, con.contextName.toStdString().c_str());

    con.module.setNewRef(Q_NULLPTR);

    if (emitSignal)
    {
        emit contextDeleted(contextId);
    }

    m_calcAccessibleContexts.removeOne(contextId);

    Py_DECREF(modules);
    Py_DECREF(dict);

    return true;
}

void
GtpyContextManager::resetContext(const GtpyContextManager::Context& type,
                                 int contextId)
{
    if (contextId < 0)
    {
        contextId = (int)type;
    }

    QString contextName = contextNameById(contextId);

    if (contextName.isEmpty())
    {
        return;
    }

    defaultContextConfig(type, contextId, contextName);
}

long
GtpyContextManager::currentPyThreadId()
{
    GTPY_GIL_SCOPE

    return PyThreadState_Get()->thread_id;
}

void
GtpyContextManager::interruptPyThread(long id)
{
    GtpyInterruptRunnable* runnable = new GtpyInterruptRunnable(id);

    QThreadPool* tp = QThreadPool::globalInstance();

    // start runnable
    tp->start(runnable);
}

void
GtpyContextManager::defaultContextConfig(
        const GtpyContextManager::Context& type,
        int contextId, const QString& contextName)
{
    GTPY_GIL_SCOPE

    PythonContext context;

    context.module = PythonQt::self()->createModuleFromScript(contextName);
    context.contextName = contextName;

    m_contextMap.insert(contextId, context);

    QString pyCode =
            "import sys\n"
            "sys.modules['" + contextName +
            "'] = None\n" +
            "del sys\n";

    context.module.evalScript(pyCode);

    m_addedObjectNames.insert(contextId, QStringList());

    specificContextConfig(type, contextId);
}

void
GtpyContextManager::specificContextConfig(
        const GtpyContextManager::Context& type, int contextId)
{
    switch (type)
    {
        case GtpyContextManager::BatchContext:
            initBatchContext(contextId);
            break;
        case GtpyContextManager::GlobalContext:
            initGlobalContext(contextId);
            break;
        case GtpyContextManager::ScriptEditorContext:
            initScriptEditorContext(contextId);
            break;
        case GtpyContextManager::CalculatorRunContext:
            initCalculatorRunContext(contextId);
            break;
        case GtpyContextManager::TaskEditorContext:
            initTaskEditorContext(contextId);
            break;
        case GtpyContextManager::TaskRunContext:
            initTaskRunContext(contextId);
            break;
        default:
            break;
    }
}

PythonQtObjectPtr
GtpyContextManager::context(int contextId) const
{
    GTPY_GIL_SCOPE

    PythonContext con = m_contextMap.value(contextId);

    return con.module;
}

//void
//GtpyContextManager::initGtObjectWrapper()
//{
//    static QString objWrapper;

//    if (objWrapper.isEmpty())
//    {
//        objWrapper =
//            "import re\n"
//            "class " + GTOBJECT_WRAPPER + ": \n"
//            "    def __init__(self, obj):\n"
//            "        self._obj = obj\n"

//            "        for i in range(len(self._obj.findGtProperties())):\n"
//            "            prop = self._obj.findGtProperties()[i]\n"
//            "            if prop.ident():\n"

//            "                funcName = re.sub('[^A-Za-z0-9]+', '', "
//                            "prop.ident())\n"
//            "                tempLetter = funcName[0]\n"
//            "                funcName = funcName.replace(tempLetter, "
//                            "tempLetter.lower(), 1)\n"

//            "                def dynGetter(self = self, propName = "
//                            "prop.ident()):\n"
//            "                    return self._obj.propertyValue(propName)\n"
//            "                setattr(self, funcName, dynGetter)\n"

//            "                tempLetter = funcName[0]\n"
//            "                funcName = funcName.replace(tempLetter, "
//                            "tempLetter.upper(), 1)\n"
//            "                funcName = 'set' + funcName\n"

//            "                def dynSetter(val, self = self, propName = "
//                            "prop.ident()):\n"
//            "                    self._obj.setPropertyValue(propName, val)\n"
//            "                setattr(self, funcName, dynSetter)\n"

//            "        helpers = " + HELPER_FAC_VAR + ".connectedHelper("
//                                        "self._obj.__class__.__name__)\n" +
//            "        for i in range(len(helpers)):\n" +
//            "            funcName = 'create' + helpers[i]\n" +

//            "            def dynCreateHelper(name, self = self, helperName = "
//                                                            "helpers[i]):\n" +
//            "                helper = " + HELPER_FAC_VAR +
//                        ".newCalculatorHelper(helperName, name, self._obj)\n" +
//            "                return " + GTOBJECT_WRAPPER + "(helper)\n" +
//            "            setattr(self, funcName, dynCreateHelper)\n" +

//            "        def dynFindGtChild(name, self = self):\n"
//            "            if hasattr(self._obj, 'findGtChild'):\n"
//            "                child = self._obj.findGtChild(name)\n"
//            "                return " + GTOBJECT_WRAPPER + "(child)\n"
//            "        setattr(self, 'findGtChild', dynFindGtChild)\n" +

//            "        def dynFindGtChildren(name = '', self = self):\n"
//            "            if hasattr(self._obj, 'findGtChildren'):\n"
//            "                children = self._obj.findGtChildren(name)\n"
//            "            retList = []\n"
//            "            for i in range(len(children)):\n" +
//            "                retList.append(" + GTOBJECT_WRAPPER +
//                             "(children[i]))\n"
//            "            return retList\n"
//            "        setattr(self, 'findGtChildren', dynFindGtChildren)\n" +

//            "    def __getattr__(self, name): \n" +
//            "        return getattr(self.__dict__['_obj'], name)\n" +

//            "    def __setattr__(self, name, value): \n" +
//            "        if name is ('_obj') or  hasattr("
//                                "self.__dict__['_obj'], name) is False or "
//                                "name.startswith('findGtChild'):\n" +
//            "            self.__dict__[name] = value\n" +
//            "        else:\n" +
//            "            setattr(self.__dict__['_obj'], name, value)\n" +
//            "    def __dir__(self): \n" +
//            "        return sorted(list(self.__dict__.keys()) + "
//                                                       "dir(self._obj))\n";
//    }

//    GTPY_GIL_SCOPE

//    PythonQtObjectPtr con =
//            PythonQt::self()->importModule(OBJECT_WRAPPER_MODULE);

//    if (!con)
//    {
//        con = PythonQt::self()->createModuleFromScript(OBJECT_WRAPPER_MODULE);
//        con.addObject(HELPER_FAC_VAR, gtCalculatorHelperFactory);
//        con.evalScript(objWrapper);
//    }
//}

void
GtpyContextManager::initCalculatorModule()
{
    static QString calcConstructors;
    static QString processElementFunction;

    if (calcConstructors.isEmpty())
    {
//        calcConstructors +=
//                "from " + OBJECT_WRAPPER_MODULE + " import " +
//                GTOBJECT_WRAPPER + "\n";
        calcConstructors +=
                "from GtObjectWrapperModuleC import GtpyExtendedWrapper\n";

        foreach (GtCalculatorData calcData,
                 gtCalculatorFactory->calculatorDataList())
        {
            if (!gtApp->devMode() &&
                    calcData->status != GtCalculatorDataImpl::RELEASE)
            {
                continue;
            }

            QString className = QString::fromUtf8(
                                    calcData->metaData().className());

            calcConstructors +=
                "def " + className + "(name = '" + calcData->id + "'):\n" +
                "    tempCalc = " + CALC_FAC_VAR + ".createCalculator(\"" +
                className + "\", name, " + TASK_VAR + ")\n" +
                "    return GtpyExtendedWrapper(tempCalc)\n";

        }

    }

    if (processElementFunction.isEmpty())
    {
        processElementFunction =
                "from PythonQt." + CLASS_WRAPPER_MODULE +
                " import GtpyProcessDataDistributor\n" +
                "from GtObjectWrapperModuleC import GtpyExtendedWrapper\n" +
                "def findGtTask(name):\n" +
                "    task = GtpyProcessDataDistributor(__task).taskElement(name)\n" +
                "    if task is not None:\n" +
                "        return task\n";
    }

    GTPY_GIL_SCOPE

    PythonQtObjectPtr calcModule =
            PythonQt::self()->createModuleFromScript(CALC_MODULE);

    static GtpyCalculatorFactory* factory = Q_NULLPTR;

    if (factory == Q_NULLPTR)
    {
        factory = new GtpyCalculatorFactory();

        factory->moveToThread(this->thread());

        factory->setParent(this);
    }

    calcModule.evalScript(TASK_VAR + QStringLiteral(" = None"));

    calcModule.addObject(CALC_FAC_VAR, factory);

    calcModule.evalScript(calcConstructors);

    calcModule.evalScript(processElementFunction);
}

void
GtpyContextManager::initLoggingModule()
{
    GTPY_GIL_SCOPE

    QString loggingClass = GtPythonLogger::staticMetaObject.className();

    m_loggingModule = PythonQt::self()->createModuleFromScript(LOGGING_MODULE);

    m_loggingModule.evalScript(QStringLiteral("from PythonQt.") +
                             CLASS_WRAPPER_MODULE + QStringLiteral(" import ") +
                             loggingClass);

    m_loggingModule.evalScript(
        QStringLiteral("class PyLogger:\n") +
        QStringLiteral("     __instance = None \n") +
        QStringLiteral("     __outputToConsole = False \n") +

        QStringLiteral("     def __init__(self, type = 0): \n") +
        QStringLiteral("         self.type = type \n") +
        QStringLiteral("         PyLogger.__instance = self \n") +

        QStringLiteral("     @staticmethod  \n") +
        QStringLiteral("     def getInstance(type): \n") +
        QStringLiteral("         if PyLogger.__instance == None: \n") +
        QStringLiteral("             PyLogger(type) \n") +
        QStringLiteral("         else: \n") +
        QStringLiteral("             PyLogger.__instance.type = type \n") +
        QStringLiteral("         return PyLogger.__instance \n") +

        QStringLiteral("     def __lshift__(self, other): \n") +
        QStringLiteral("         if self.type is 0: \n") +
        QStringLiteral("             print (\"[DEBUG] \" + str(other)) \n") +
        QStringLiteral("             if PyLogger.__outputToConsole:\n") +
        QStringLiteral("                 GtPythonLogger.gtPyDebug(other)\n") +
        QStringLiteral("             return \n") +
        QStringLiteral("         if self.type is 1: \n") +
        QStringLiteral("             print (\"[INFO] \" + str(other)) \n") +
        QStringLiteral("             if PyLogger.__outputToConsole:\n") +
        QStringLiteral("                 GtPythonLogger.gtPyInfo(other)\n") +
        QStringLiteral("             return \n") +
        QStringLiteral("         if self.type is 2: \n") +
        QStringLiteral("             print (\"[ERROR] \" + str(other)) \n") +
        QStringLiteral("             if PyLogger.__outputToConsole:\n") +
        QStringLiteral("                 GtPythonLogger.gtPyError(other)\n") +
        QStringLiteral("             return \n") +
        QStringLiteral("         if self.type is 3: \n") +
        QStringLiteral("             print (\"[FATAL] \" + str(other)) \n") +
        QStringLiteral("             if PyLogger.__outputToConsole:\n") +
        QStringLiteral("                 GtPythonLogger.gtPyFatal(other)\n") +
        QStringLiteral("             return \n") +
        QStringLiteral("         if self.type is 4: \n") +
        QStringLiteral("             print (\"[WARNING] \" + str(other)) \n") +
        QStringLiteral("             if PyLogger.__outputToConsole:\n") +
        QStringLiteral("                 GtPythonLogger.gtPyWarning(other)\n") +
        QStringLiteral("             return \n"));


    m_loggingModule.evalScript(QStringLiteral("def gtDebug(): \n") +
                   QStringLiteral("     return PyLogger.getInstance(0)\n"));

    m_loggingModule.evalScript(QStringLiteral("def gtInfo(): \n") +
                   QStringLiteral("     return PyLogger.getInstance(1)\n"));

    m_loggingModule.evalScript(QStringLiteral("def gtError(): \n") +
                   QStringLiteral("     return PyLogger.getInstance(2)\n"));

    m_loggingModule.evalScript(QStringLiteral("def gtFatal(): \n") +
                   QStringLiteral("     return PyLogger.getInstance(3)\n"));

    m_loggingModule.evalScript(QStringLiteral("def gtWarning(): \n") +
                   QStringLiteral("     return PyLogger.getInstance(4)\n"));
}

void
GtpyContextManager::initImportBehaviour()
{
    QString finderLoader;

    if (pythonVersion().startsWith("3."))
    {
        finderLoader =
           "from importlib.machinery import ModuleSpec\n"
           "import sys\n"
           "import inspect\n"

           "class DataPackageFinder:\n"

           "    @classmethod\n"
           "    def find_spec(cls, fullname, path=None, target=None):\n"
           "        name_parts = fullname.split('.')\n"
           "        if name_parts[:1] != ['" + CALC_MODULE + "']:\n"
           "            return None\n"
           "        else:\n"
           "            return ModuleSpec(fullname, DataPackageImporter())\n\n"


           "class DataPackageImporter:\n"

           "    @classmethod\n"
           "    def create_module(cls, spec):\n"
           "        current_frame = inspect.currentframe()\n"
           "        while current_frame is not None:\n"

           "            if '"+ CALC_MODULE + "' in current_frame.f_globals:\n"
           "                calcMod = current_frame.f_globals['" +
                           CALC_MODULE + "']\n"
           "                return calcMod\n"
           "            current_frame = current_frame.f_back\n"
           "        raise ImportError(\"It is not allowed to import the module "
                        "'" + CALC_MODULE + "' here!\")\n"
           "        return None\n"

           "    @classmethod\n"
           "    def exec_module(cls, module):\n"
           "        return module\n";
    }
    else
    {
        finderLoader =
           "import sys\n"
           "import inspect\n"

           "class DataPackageFinder:\n"

           "    @classmethod\n"
           "    def find_module(cls, fullname, path=None):\n"
           "        name_parts = fullname.split('.')\n"
           "        if name_parts[:1] != ['" + CALC_MODULE + "']:\n"
           "            return None\n"
           "        else:\n"
           "            return DataPackageImporter()\n"

           "class DataPackageImporter:\n"

           "    @classmethod\n"
           "    def load_module(cls, fullname):\n"
           "        current_frame = inspect.currentframe()\n"

           "        while current_frame is not None:\n"
           "            if '"+ CALC_MODULE + "' in current_frame.f_globals:\n"
           "                calcMod = current_frame.f_globals['" +
                           CALC_MODULE + "']\n"
           "                return calcMod\n"
           "            current_frame = current_frame.f_back\n"
           "        raise ImportError(\"It is not allowed to import the module "
                        "'" + CALC_MODULE + "' here!\")\n"
           "        return None\n";
    }

    QString impBehaviour =
            DEFAULT_IMPORT + " =  __builtins__['__import__']\n" +
            "import sys\n"
            "from " + CUSTOM_IMPORT_MODULE + " import DataPackageFinder\n"
            "sys.meta_path.append(DataPackageFinder)\n"
            "del DataPackageFinder\n";

    GTPY_GIL_SCOPE

    PythonQtObjectPtr customImp = PythonQt::self()->createModuleFromScript(
                            CUSTOM_IMPORT_MODULE);

    customImp.evalScript(finderLoader);

    PythonQtObjectPtr defaultImp = PythonQt::self()->createModuleFromScript(
                            DEFAULT_IMPORT_MODULE);

    defaultImp.evalScript(impBehaviour);

    PyObject* builtins = PyDict_GetItemString(PyModule_GetDict(defaultImp),
                                              "__builtins__");

    PyObject* defImp = PyDict_GetItemString(builtins, "__import__");

    PythonQtObjectPtr imp = GtpyMyImport_Type.tp_new(&GtpyMyImport_Type, NULL,
                                                     NULL);

    ((GtpyMyImport*)imp.object())->defaultImp = defImp;
    PyDict_SetItemString(builtins, "__import__", imp);
}

void
GtpyContextManager::initBatchContext(int contextId)
{
    PythonQtObjectPtr con = context(contextId);

    if (con == Q_NULLPTR)
    {
        return;
    }

    importDefaultModules(contextId);

    if (gtApp != Q_NULLPTR)
    {
        addObject(contextId, "GTlab", gtApp);

        QString pyCode =
            "def openProject(projectName):\n"
            "    return GTlab.openProject(projectName)\n"
            "def currentProject():\n"
            "    return GTlab.currentProject()\n"
            "def init(id = ''):\n"
            "    return GTlab.init(id)\n"
            "def switchSession(id = ''):\n"
            "    return GTlab.switchSession(id)\n";

        GTPY_GIL_SCOPE
        con.evalScript(pyCode);
    }
    else
    {
        qWarning() << tr("ERROR: GtApplication is not initialized!!! "
                         "Batch context can not register the GTlab object.");
    }

    importLoggingFuncs(contextId, true);
}

void
GtpyContextManager::initGlobalContext(int contextId)
{
    PythonQtObjectPtr con = context(contextId);

    if (con == Q_NULLPTR)
    {
        return;
    }

    importDefaultModules(contextId);

    if (gtApp != Q_NULLPTR)
    {
        addObject(contextId, QStringLiteral("GTlab"), gtApp);

        QString pyCode =
            "def openProject(projectName):\n"
            "    return GTlab.openProject(projectName)\n"
            "def currentProject():\n"
            "    return GTlab.currentProject()\n"
            "def init(id = ''):\n"
            "    return GTlab.init(id)\n"
            "def switchSession(id = ''):\n"
            "    return GTlab.switchSession(id)\n";

        GTPY_GIL_SCOPE

        con.evalScript(pyCode);
    }
    else
    {
        qWarning() << tr("ERROR: GtApplication is not initialized!!! "
                         "Batch context can not register the GTlab object.");
    }

    QString pyCode =
        QStringLiteral("import sys\n") +
        QStringLiteral("sys.argv.append('')\n") +
        QStringLiteral("del sys\n");

    GTPY_GIL_SCOPE

    con.evalScript(pyCode);

    importLoggingFuncs(contextId, true);
}

void
GtpyContextManager::initScriptEditorContext(int contextId)
{
    importDefaultModules(contextId);

    importLoggingFuncs(contextId, false);
}

void
GtpyContextManager::initCalculatorRunContext(int contextId)
{
    importDefaultModules(contextId);

    importLoggingFuncs(contextId, true);
}

void
GtpyContextManager::initTaskEditorContext(int contextId)
{
    importDefaultModules(contextId);

    importLoggingFuncs(contextId, false);

    importCalcModule(contextId);
}

void
GtpyContextManager::initTaskRunContext(int contextId)
{
    importDefaultModules(contextId);

    importLoggingFuncs(contextId, true);

    importCalcModule(contextId);
}

void
GtpyContextManager::initStdOut()
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr sys;
    sys.setNewRef(PyImport_ImportModule("sys"));

    // create a redirection object for stdout and stderr
    m_out = GtpyStdOutRedirect_Type.tp_new(&GtpyStdOutRedirect_Type,NULL, NULL);
    ((GtpyStdOutRedirect*)m_out.object())->callback = stdOutRedirectCB;

    m_err = GtpyStdOutRedirect_Type.tp_new(&GtpyStdOutRedirect_Type,NULL, NULL);
    ((GtpyStdOutRedirect*)m_err.object())->callback = stdErrRedirectCB;

    // replace the built in file objects with the new objects
    PyModule_AddObject(sys, "stdout", m_out);
    PyModule_AddObject(sys, "stderr", m_err);
}

void
GtpyContextManager::initWrapperModule()
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr sys;
    sys.setNewRef(PyImport_ImportModule("sys"));

    QByteArray name = "GtObjectWrapperModuleC";
    PyObject* myMod;
#ifdef PY3K
    customPyModule.m_name = name.constData();
    myMod = PyModule_Create(&customPyModule);
#else
    myMod = Py_InitModule(name.constData(), NULL);
#endif
    // add GtObjectWrapperModuleC to the list of builtin module names
    PyObject* old_module_names =
            PyObject_GetAttrString(sys.object(),"builtin_module_names");

    if (old_module_names && PyTuple_Check(old_module_names))
    {
        Py_ssize_t old_size = PyTuple_Size(old_module_names);
        PyObject* module_names = PyTuple_New(old_size + 1);

        for (Py_ssize_t i = 0; i < old_size; i++)
        {
            PyTuple_SetItem(module_names, i,
                            PyTuple_GetItem(old_module_names, i));
        }

        PyTuple_SetItem(module_names, old_size,
                        PyString_FromString(name.constData()));
        PyModule_AddObject(sys.object(), "builtin_module_names", module_names);
    }

//    Py_XDECREF(old_module_names);

#ifdef PY3K
    PyDict_SetItem(PyObject_GetAttrString(sys.object(), "modules"),
                   PyUnicode_FromString(name.constData()), myMod);
#endif

    if (PyModule_AddObject(myMod, "GtpyExtendedWrapper",
                           (PyObject*) &GtpyExtendedWrapper_Type) < 0)
    {
        Py_DECREF(&GtpyExtendedWrapper_Type);
        Py_DECREF(myMod);
    }
}

void
GtpyContextManager::initTaskFinderModule()
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr sys;
    sys.setNewRef(PyImport_ImportModule("sys"));

    QByteArray name = GtpyGlobals::GTPYTASKFINDER_MODULE.toUtf8();

    PyObject* myMod;
#ifdef PY3K
    customPyModule.m_name = name.constData();
    myMod = PyModule_Create(&GtpyTaskFinder_Module);
#else
    myMod = Py_InitModule(name.constData(), GtpyTaskFinder_StaticMethods);
#endif
    // add GtTaskFinder to the list of builtin module names
    PyObject* old_module_names =
            PyObject_GetAttrString(sys.object(),"builtin_module_names");

    if (old_module_names && PyTuple_Check(old_module_names))
    {
        Py_ssize_t old_size = PyTuple_Size(old_module_names);
        PyObject* module_names = PyTuple_New(old_size + 1);

        for (Py_ssize_t i = 0; i < old_size; i++)
        {
            PyTuple_SetItem(module_names, i,
                            PyTuple_GetItem(old_module_names, i));
        }

        PyTuple_SetItem(module_names, old_size,
                        PyString_FromString(name.constData()));
        PyModule_AddObject(sys.object(), "builtin_module_names", module_names);
    }

#ifdef PY3K
    PyDict_SetItem(PyObject_GetAttrString(sys.object(), "modules"),
                   PyUnicode_FromString(name.constData()), myMod);
#endif
}

void
GtpyContextManager::importDefaultModules(int contextId)
{
    PythonQtObjectPtr con = context(contextId);

    if (con == Q_NULLPTR)
    {
        return;
    }

    QString pyCode =
        "from PythonQt import " + CLASS_WRAPPER_MODULE + "\n" +
        "from PythonQt import QtCore\n"
        "import " + DEFAULT_IMPORT_MODULE + "\n"
        "from " + GtpyGlobals::GTOBJECT_WRAPPER_MODULE + " import " +
            GtpyGlobals::GTOBJECT_WRAPPER + "\n";

    GTPY_GIL_SCOPE

    con.evalScript(pyCode);
}

void
GtpyContextManager::importLoggingFuncs(int contextId,
                                       bool appConsole)
{
    PythonQtObjectPtr con = context(contextId);

    if (con == Q_NULLPTR)
    {
        return;
    }

    QString pyCode =
        QStringLiteral("from ") + LOGGING_MODULE +
            QStringLiteral(" import gtDebug\n") +
        QStringLiteral("from ") + LOGGING_MODULE +
            QStringLiteral(" import gtInfo\n") +
        QStringLiteral("from ") + LOGGING_MODULE +
            QStringLiteral(" import gtError\n") +
        QStringLiteral("from ") + LOGGING_MODULE +
            QStringLiteral(" import gtFatal\n") +
        QStringLiteral("from ") + LOGGING_MODULE +
            QStringLiteral(" import gtWarning\n");

    GTPY_GIL_SCOPE

    con.evalScript(pyCode);

    m_appLogging.insert(contextId, appConsole);
}

void
GtpyContextManager::importCalcModule(int contextId)
{
    if (!m_calcAccessibleContexts.contains(contextId))
    {
        m_calcAccessibleContexts << contextId;
    }

    PythonQtObjectPtr con = context(contextId);

    if (con == Q_NULLPTR)
    {
        return;
    }

    initCalculatorModule();

    QString pyCode =
        "import sys\n"
        "if '" + CALC_MODULE + "' in sys.modules and '" + DEFAULT_IMPORT_MODULE +
            "' in sys.modules:\n"
        "    if sys.modules['" + CALC_MODULE + "'] is not None and " +
            "sys.modules['" + DEFAULT_IMPORT_MODULE + "'] is not None:\n"
        "        import " + DEFAULT_IMPORT_MODULE + "\n" +
        "        " + CALC_MODULE + " = " + DEFAULT_IMPORT_MODULE + "." +
            DEFAULT_IMPORT + "('" + CALC_MODULE + "')\n"
        "        from " + CALC_MODULE + " import *\n"
        "        del " + CALC_FAC_VAR + "\n"
        "        if '" + CALC_MODULE + "' in sys.modules:\n"
        "            del sys.modules['" + CALC_MODULE + "']\n"
        "        del sys\n";

    GTPY_GIL_SCOPE

    con.evalScript(pyCode);
}

//void
//GtpyContextManager::registerCalcModuleInSys(int contextId)
//{
//    PythonQtObjectPtr con = context(contextId);

//    if (con == Q_NULLPTR)
//    {
//        return;
//    }

//    QString pyCode =
//            "if '" + CALC_MODULE + "' in locals():\n" +
//            "    import sys\n" +
//            "    sys.modules['" + CALC_MODULE + "'] = " + CALC_MODULE + "\n" +
//            "    del sys\n";

//    GTPY_GIL_SCOPE

//    con.evalScript(pyCode);
//}

//void
//GtpyContextManager::removeCalcModuleFromSys()
//{
//    PythonQtObjectPtr con = context(GtpyContextManager::GlobalContext);

//    if (con == Q_NULLPTR)
//    {
//        return;
//    }

//    QString pyCode =
//        "import sys\n"
//        "if '" + CALC_MODULE + "' in sys.modules:\n" +
//        "    sys.modules['" + CALC_MODULE + "'] = None\n" +
//        "del sys\n";

//    GTPY_GIL_SCOPE

//    con.evalScript(pyCode);
//}

int
GtpyContextManager::lineOutOfMessage(const QString& message)
{
    bool ok = false;

    int lineNumber = message.toInt(&ok);

    if (ok)
    {
        return lineNumber;
    }

    QRegularExpression errorLine(QStringLiteral("\"<string>\", line \\d+"));

    if (message.indexOf(errorLine) == -1)
    {
        return -1;
    }

    QRegularExpressionMatch match = errorLine.match(message);

    lineNumber = match.captured().mid(17).toInt(&ok);

    if (ok)
    {
        return lineNumber ;
    }
    else
    {
        return -1;
    }
}

void
GtpyContextManager::setMetaDataToThreadDict(int contextId, bool output,
                                            bool error)
{
    GTPY_GIL_SCOPE

    PyObject* threadDict = PyThreadState_GetDict();

    if (!threadDict)
    {
        return;
    }

    Py_INCREF(threadDict);

    QString contextName = contextNameById(contextId);

    PyObject* key = PyString_FromString(contextName.toStdString().c_str());

    PyDict_SetItemString(threadDict, GtpyStdOut::CONTEXT_KEY, key);

    Py_DECREF(key);

    PyObject* outputVal = PyLong_FromLong(output ? 1 : 0);

    PyDict_SetItemString(threadDict, GtpyStdOut::OUTPUT_KEY, outputVal);

    Py_DECREF(outputVal);

    PyObject* errorVal = PyLong_FromLong(error ? 1 : 0);

    PyDict_SetItemString(threadDict, GtpyStdOut::ERROR_KEY, errorVal);

    Py_DECREF(errorVal);

    Py_DECREF(threadDict);
}

QMultiMap<QString, GtpyFunction>
GtpyContextManager::introspectObject(PyObject* object) const
{
    GTPY_GIL_SCOPE

    QMultiMap<QString, GtpyFunction> results;

    if (m_decorator == Q_NULLPTR)
    {
        return results;
    }

    if (object == Q_NULLPTR)
    {
        return results;
    }

    Py_INCREF(object);

    PyObject* keys = NULL;
    bool isDict = false;

    if (PyDict_Check(object))
    {
        keys = PyDict_Keys(object);
        isDict = true;
    }
    else
    {
        keys = PyObject_Dir(object);
    }

    if (keys)
    {
        int count = PyList_Size(keys);
        PyObject* value = Q_NULLPTR;
        PyObject* key = Q_NULLPTR;
        QString keystr;

        for (int i = 0; i < count; i++)
        {
            key = PyList_GetItem(keys, i);

            if (!key)
            {
                continue;
            }

            Py_INCREF(key);

            if (isDict)
            {
                value = PyDict_GetItem(object, key);
                Py_XINCREF(value);
            }
            else
            {
                value = PyObject_GetAttr(object, key);
            }

            if (!value)
            {
                continue;
            }

            const char* keychar = PyString_AsString(key);
            keystr = QString::fromUtf8(keychar);

            static const QString underscoreStr(QStringLiteral("__"));

            if (!keystr.startsWith(underscoreStr))
            {
                GtpyFunction func;

                if (value->ob_type == &PythonQtSlotFunction_Type)
                {
                    func.name = keystr + QStringLiteral("()");

                    PythonQtSlotInfo* info =
                        ((PythonQtSlotFunctionObject*)value)->m_ml;

                    if (info)
                    {
                        QString type;

                        if (info->parameterCount() > 0)
                        {
                            type = info->parameters()[0].name;
                        }

                        QString fullSig = info->fullSignature(true);

                        func.toolTip = type + " " + fullSig;
                    }
                    else
                    {
                        func.toolTip = func.name;
                    }

                    func.completion = func.name;
                }
                else if (value->ob_type == &PyCFunction_Type ||
                         value->ob_type == &PyFunction_Type ||
                         value->ob_type == &PyMethod_Type)
                {
                    func.name = keystr + QStringLiteral("()");
                    func.toolTip = func.name;
                    func.completion = func.name;

                }
                else
                {
                    func.name = keystr;
                    func.toolTip = func.name;
                    func.completion = func.name;
                }

                results.insert(keystr.toLower(), func);
            }

            Py_DECREF(value);
            Py_DECREF(key);
        }

        Py_DECREF(keys);
    }

//    if (PyObject_TypeCheck(object, &PythonQtInstanceWrapper_Type))
//    {
    PythonQtObjectPtr p = object;

    QString childrenFunc = m_decorator->getFunctionName(
                               GET_CHILDREN_TAG);

    if (results.keys().contains(childrenFunc.toLower()))
    {
        results.remove(childrenFunc.toLower());
        QVariant childrenVar = p.call(childrenFunc);

        QList<QVariant> childrenListVar = childrenVar.toList();

        if (!childrenListVar.isEmpty())
        {
            foreach (QVariant var, childrenListVar)
            {
                QObject* obj = qvariant_cast<QObject*>(var);

                if (obj != Q_NULLPTR && !obj->objectName().isEmpty())
                {
                    QString objName = obj->objectName();

                    QString completion;

                    QString desc;

                    GtpyFunction existing = results.value(
                                                objName.toLower());

                    if (!existing.name.isEmpty())
                    {
                        results.remove(objName.toLower());

                        QString funcName = m_decorator->getFunctionName(
                                               FIND_GT_CHILDREN_TAG);

                        if (funcName.isEmpty())
                        {
                            completion = objName;

                            desc = FUNCTION_WARNING;
                        }
                        else
                        {
                            completion = funcName + QStringLiteral("(\"") +
                                         objName + QStringLiteral("\")");

                            desc = LIST_DATATYPE + QStringLiteral("<") +
                                   QString::fromUtf8(
                                   QObject::staticMetaObject.className()) +
                                   QStringLiteral("*> ") + completion;
                        }
                    }
                    else
                    {
                        if (!objName.contains(
                                QRegExp(QStringLiteral("^[a-zA-Z0-9_]*$"))))
                        {
                            QString funcName = m_decorator->getFunctionName(
                                                   FIND_GT_CHILD_TAG);

                            if (funcName.isEmpty())
                            {
                                completion = objName;

                                desc = FUNCTION_WARNING;
                            }
                            else
                            {
                                completion = funcName +
                                             QStringLiteral("(\"") +
                                            objName + QStringLiteral("\")");

                                desc = QString::fromUtf8(
                                           obj->metaObject()->className()) +
                                       QStringLiteral("* ") + completion;
                            }
                        }
                        else
                        {
                            completion = objName;

                            desc = QString::fromUtf8(obj->metaObject()->
                                                     className())
                                   + QStringLiteral("* ") +
                                   obj->objectName();
                        }
                    }

                    GtpyFunction func;
                    func.name = objName;
                    func.toolTip = desc;
                    func.completion = completion;

                    results.insert(func.name.toLower(), func);
                }
            }
        }
    }

    QString findPropsFuncName = m_decorator->getFunctionName(
                                    FIND_GT_PROPERTIES_TAG);

    if (results.keys().contains(findPropsFuncName.toLower()))
    {
        QVariant propertyVar = p.call(findPropsFuncName);

        QList<QVariant> propertyListVar = propertyVar.toList();

        if (!propertyListVar.isEmpty())
        {
            foreach (QVariant var, propertyListVar)
            {
                GtAbstractProperty* prop = qvariant_cast<GtAbstractProperty*>(
                                               var);

                if (prop != Q_NULLPTR && !prop->objectName().isEmpty())
                {
                    QString propName = prop->objectName();

                    QString completion;

                    QString desc;

                    QString findFuncName = m_decorator->getFunctionName(
                                               FIND_GT_PROPERTY_TAG);

                    if (findFuncName.isEmpty())
                    {
                        completion = prop->objectName();

                        desc = FUNCTION_WARNING;
                    }
                    else
                    {
                        completion = findFuncName + QStringLiteral("(\"") +
                                     prop->ident() + QStringLiteral("\")");

                        desc = QString::fromUtf8(prop->metaObject()->
                                                 className())
                               + QStringLiteral("* ") + completion;
                    }

                    GtpyFunction propfunc;
                    propfunc.name = propName;
                    propfunc.completion = completion;
                    propfunc.toolTip = desc;

                    results.insert(propfunc.name.toLower(), propfunc);

                    propName =  prop->objectName() +
                                QStringLiteral(" setValue");

                    QString setFuncName = m_decorator->getFunctionName(
                                              SET_PROPERTY_VALUE_TAG);

                    if (setFuncName.isEmpty())
                    {
                        completion = prop->objectName();

                        desc = FUNCTION_WARNING;
                    }
                    else
                    {
                        completion = setFuncName + QStringLiteral("(\"") +
                                     prop->ident() +
                                     QStringLiteral("\", )");

                        desc = QStringLiteral("void ") + setFuncName +
                               QStringLiteral("(\"") + prop->ident() +
                               QStringLiteral("\",") + VARIANT_DATATYPE +
                               QStringLiteral("())");
                    }

                    GtpyFunction setValfunc;
                    setValfunc.name = propName;
                    setValfunc.completion = completion;
                    setValfunc.toolTip = desc;

                    results.insert(setValfunc.name.toLower(), setValfunc);

                    propName = prop->objectName() +
                               QStringLiteral(" Value");



                    QString getFuncName = m_decorator->getFunctionName(
                                              PROPERTY_VALUE_TAG);

                    if (getFuncName.isEmpty())
                    {
                        completion = prop->objectName();

                        desc = FUNCTION_WARNING;
                    }
                    else
                    {
                        completion = getFuncName + QStringLiteral("(\"") +
                                     prop->ident() + QStringLiteral("\")");

                        desc = QStringLiteral("QVariant ") + completion;
                    }

                    GtpyFunction valfunc;
                    valfunc.name = propName;
                    valfunc.completion = completion;
                    valfunc.toolTip = desc;

                    results.insert(valfunc.name.toLower(), valfunc);
                }
            }
        }
    }
//    }

    Py_DECREF(object);

    return results;
}

QMultiMap<QString, GtpyFunction>
GtpyContextManager::customCompletions() const
{
    QMultiMap<QString, GtpyFunction> results;

    QString import = QStringLiteral("import");
    GtpyFunction importKeyword;
    importKeyword.completion = import + QStringLiteral(" ");
    importKeyword.name = import;
    importKeyword.toolTip = import + QStringLiteral(" [keyword]");
    importKeyword.cursorOffset = 0;

    results.insert(import, importKeyword);

    QString from = QStringLiteral("from");
    GtpyFunction fromKeyword;
    fromKeyword.completion = from + QStringLiteral(" ");
    fromKeyword.name = from;
    fromKeyword.toolTip = from + QStringLiteral(" [keyword]");
    fromKeyword.cursorOffset = 0;

    results.insert(from, fromKeyword);

    QString print = QStringLiteral("print");
    GtpyFunction printFunc;
    printFunc.completion = print + QStringLiteral(" ()");
    printFunc.name = print + QStringLiteral("()");
    printFunc.toolTip = print + QStringLiteral("() [built-in]");
    printFunc.cursorOffset = 1;

    results.insert(print, printFunc);

    QString forloop = QStringLiteral("for i in range():");
    GtpyFunction forloopFunc;
    forloopFunc.completion = forloop;
    forloopFunc.name = forloop;
    forloopFunc.toolTip = forloop;
    forloopFunc.cursorOffset = 2;

    results.insert(forloop, forloopFunc);

    QString forloopContainer = QStringLiteral("for x in :");
    GtpyFunction forloopContainerFunc;
    forloopContainerFunc.completion = forloopContainer;
    forloopContainerFunc.name = forloopContainer;
    forloopContainerFunc.toolTip = forloopContainer;
    forloopContainerFunc.cursorOffset = 1;

    results.insert(forloopContainer, forloopContainerFunc);

    return results;
}

QMultiMap<QString, GtpyFunction>
GtpyContextManager::builtInCompletions(int contextId) const
{
    GTPY_GIL_SCOPE

    QMultiMap<QString, GtpyFunction> results;

    // the script down below does not work in 2.7
    if (pythonVersion() != QStringLiteral("3.7"))
    {
        return results;
    }

    PythonQtObjectPtr con = context(contextId);

    if (con.isNull())
    {
        return results;
    }

    con.evalScript(QStringLiteral("import inspect"));
    con.evalScript(QStringLiteral("def __builtins():") +
                       QStringLiteral("return dir(inspect.builtins)"));

    QVariant v = con.call(QStringLiteral("__builtins"));

    con.evalScript(QStringLiteral("del inspect"));

    if (v.isNull())
    {
        return results;
    }

    QStringList builtinFunctions = v.toStringList();

    foreach (QString name, builtinFunctions)
    {
        if (name.startsWith(QStringLiteral("_")))
        {
            continue;
        }

        QChar c = name.at(0).toUpper();

        if (name.startsWith(c, Qt::CaseSensitive))
        {
            continue;
        }

        QString builtIn = name + QStringLiteral("()");
        GtpyFunction builtInFunc;
        builtInFunc.completion = builtIn;
        builtInFunc.name = builtIn;
        builtInFunc.toolTip = builtIn + QStringLiteral(" [built-in]");

        results.insert(name, builtInFunc);
    }

    return results;
}

void
GtpyContextManager::setImportableModulesCompletions(int contextId)
{
    GTPY_GIL_SCOPE

    // the script down below does not work in 2.7
    if (pythonVersion() != QStringLiteral("3.7"))
    {
        return;
    }

    QMultiMap<QString, GtpyFunction> results;

    PythonQtObjectPtr con = context(contextId);

    if (con.isNull())
    {
        return;
    }

    con.evalScript(QStringLiteral("import pkgutil"));
    con.evalScript(QStringLiteral("def __importableModules():\n") +
                       QStringLiteral("\tmodules = pkgutil.iter_modules()\n") +
                       QStringLiteral("\tx = (i.name for i in modules)\n") +
                       QStringLiteral("\treturn list(x)"));

    QVariant v = con.call(QStringLiteral("__importableModules"));

    con.evalScript(QStringLiteral("del pkgutil"));

    if (v.isNull())
    {
        return;
    }

    QStringList modules = v.toStringList();

    modules.append(QStringLiteral("sys"));  // sys is somehow not listed
    modules.append(QStringLiteral("time")); // time is somehow not listed

    foreach (QString name, modules )
    {
        if (name.startsWith(name.startsWith(QStringLiteral("_"))))
        {
            continue;
        }

        // import package
        QString import = QStringLiteral("import ") + name;
        GtpyFunction importModule;
        importModule.completion = import;
        importModule.name = import;
        importModule.toolTip = import + QStringLiteral(" [package]");

        results.insert(import, importModule);

        // from package import
        QString fromModule = QStringLiteral("from ") + name +
                             QStringLiteral(" import ");
        GtpyFunction fromModuleImport;
        fromModuleImport.completion = fromModule;
        fromModuleImport.name = fromModule;
        fromModuleImport.toolTip = fromModule + QStringLiteral(" [package]");

        results.insert(fromModule, fromModuleImport);
    }

    m_importableModulesCompletions = results;
}

QMultiMap<QString, GtpyFunction>
GtpyContextManager::calculatorCompletions(int contextId) const
{
    QMultiMap<QString, GtpyFunction> results;

    if (!m_calcAccessibleContexts.contains(contextId))
    {
        return results;
    }

    foreach (GtCalculatorData calcData,
             gtCalculatorFactory->calculatorDataList())
    {
        if (!gtApp->devMode() &&
                calcData->status != GtCalculatorDataImpl::RELEASE)
        {
            continue;
        }

        QString calcId = calcData->id;

        GtpyFunction calc;
        calc.completion = QString::fromUtf8(calcData->metaData().className()) +
                          QStringLiteral("(\"") + calcId +
                          QStringLiteral("\")");
        calc.name = calcId;
        calc.toolTip = QString::fromUtf8(calcData->metaData().className()) +
                       QStringLiteral("(\"") + calcId + QStringLiteral("\")");

        results.insert(calcId, calc);
    }

    return results;
}

void
GtpyContextManager::setStandardCompletions(int contextId)
{
    QMultiMap<QString, GtpyFunction> results = builtInCompletions(contextId);
    QMultiMap<QString, GtpyFunction> customs = customCompletions();

    foreach (QString name, customs.keys()) // remove duplicates (mainly print)
    {
        results.remove(name);
    }

    results += customCompletions();

    m_standardCompletions = results;
}

void
GtpyContextManager::registerTypeConverters() const
{
    int objectPtrMapId = qRegisterMetaType<QMap<int, double>>(
                "QMap<int, double>");

    PythonQtConv::registerMetaTypeToPythonConverter(objectPtrMapId,
                         GtpyTypeConversion::convertFromQMapIntDouble);
    PythonQtConv::registerPythonToMetaTypeConverter(objectPtrMapId,
                         GtpyTypeConversion::convertToQMapIntDouble);

    objectPtrMapId = qRegisterMetaType<QMap<QString, double>>(
                "QMap<QString, double>");

    PythonQtConv::registerMetaTypeToPythonConverter(objectPtrMapId,
                         GtpyTypeConversion::convertFromQMapStringDouble);
    PythonQtConv::registerPythonToMetaTypeConverter(objectPtrMapId,
                         GtpyTypeConversion::convertToQMapStringDouble);

    objectPtrMapId = qRegisterMetaType<QMap<QString, int>>(
                "QMap<QString, int>");

    PythonQtConv::registerMetaTypeToPythonConverter(objectPtrMapId,
                         GtpyTypeConversion::convertFromQMapStringInt);
    PythonQtConv::registerPythonToMetaTypeConverter(objectPtrMapId,
                         GtpyTypeConversion::convertToQMapStringInt);
}

QString
GtpyContextManager::pythonVersion() const
{
    QString version;

    version = QString::number(PY_MAJOR_VERSION) + "." +
              QString::number(PY_MINOR_VERSION);

    return version;
}

void
GtpyContextManager::stdOutRedirectCB(const QString& contextName,
                                     const bool output, const bool /*error*/,
                                     const QString& message)
{
    if (!GtpyContextManager::instance())
    {
        std::cout << message.toLatin1().data() << std::endl;
        return;
    }

    if (output && !contextName.isEmpty())
    {
        int contextId = GtpyContextManager::instance()->
                        contextIdByName(contextName);

        if (contextId == BatchContext)
        {
            if (message.indexOf(QStringLiteral("\n")) != 0 &&
                    !message.isEmpty())
            {
                std::cout << message.toLatin1().data() << std::endl;
            }

            return;
        }

        emit GtpyContextManager::instance()->pythonMessage(message, contextId);
    }
//    else
//    {
//        std::cout << message.toLatin1().data() << std::endl;
//    }
}

void
GtpyContextManager::stdErrRedirectCB(const QString& contextName,
                                     const bool /*output*/, const bool error,
                                     const QString& message)
{
    if (!GtpyContextManager::instance())
    {
        std::cerr << message.toLatin1().data() << std::endl;
        return;
    }

    if (error && !contextName.isEmpty())
    {
        int contextId = GtpyContextManager::instance()->
                        contextIdByName(contextName);

        emit GtpyContextManager::instance()->errorMessage(message, contextId);

        int line = lineOutOfMessage(message);

        if (line > -1)
        {
            emit GtpyContextManager::instance()->errorCodeLine(line, contextId);
        }
    }
//    else
//    {
//        std::cout << message.toLatin1().data() << std::endl;
//    }
}

int
GtpyContextManager::contextIdByName(const QString& contextName)
{
    GTPY_GIL_SCOPE

    QList<int> ids = m_contextMap.keys();

    foreach (int id, ids)
    {
        PythonContext con = m_contextMap.value(id);

        if (con.contextName == contextName)
        {
            return id;
        }
    }

    return -1;
}

QString
GtpyContextManager::contextNameById(int contextId)
{
    GTPY_GIL_SCOPE

    return m_contextMap.value(contextId).contextName;
}

void
GtpyContextManager::onErrorMessage(const QString& message)
{
    GTPY_GIL_SCOPE

    PyObject* threadDict = PyThreadState_GetDict();

    if (!threadDict)
    {
        return;
    }

    Py_INCREF(threadDict);

    PyObject* item = PyDict_GetItem(threadDict, PyString_FromString(
                                        GtpyStdOut::CONTEXT_KEY));

    int contextId = -1;

    if (item)
    {
        Py_INCREF(item);

        const char* val = PyString_AsString(item);

        contextId = contextIdByName(QString(val));

        Py_DECREF(item);
    }

    item = PyDict_GetItem(threadDict, PyString_FromString(
                              GtpyStdOut::ERROR_KEY));

    Py_XINCREF(item);

    bool error = false;

    if (item && PyInt_Check(item))
    {
        error = (bool)PyInt_AsLong(item);
    }

    if (error && contextId > -1)
    {
        emit errorMessage(message, contextId);
    }

    Py_XDECREF(item);
    Py_DECREF(threadDict);
}

void
GtpyContextManager::onSystemExitExceptionRaised(const int /*exep*/) const
{
    //Has to exist to keep GTlab running when a python script calls sys.exit()
}

PyObject*
GtpyTypeConversion::convertFromQMapIntDouble(const void* inObject, const int)
{
    return mapToPython<int, double>(inObject);
}

bool
GtpyTypeConversion::convertToQMapIntDouble(PyObject* obj, void* outMap,
                                           int, bool)
{
    return pythonToMap<int, double> (obj, outMap);
}

PyObject*
GtpyTypeConversion::convertFromQMapStringDouble(const void* inObject, int)
{
    return mapToPython<QString, double>(inObject);
}

bool
GtpyTypeConversion::convertToQMapStringDouble(PyObject* obj, void* outMap,
                                              int, bool)
{
    return pythonToMap<QString, double>(obj, outMap);
}

PyObject*
GtpyTypeConversion::convertFromQMapStringInt(const void* inObject, int)
{
    return mapToPython<QString, int>(inObject);
}

bool
GtpyTypeConversion::convertToQMapStringInt(PyObject* obj, void* outMap,
                                           int, bool)
{
    return pythonToMap<QString, int>(obj, outMap);
}
