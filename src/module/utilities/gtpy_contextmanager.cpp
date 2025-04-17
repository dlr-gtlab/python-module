/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_contextmanager.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <Qt>

#ifdef Q_OS_LINUX
#include <dlfcn.h>
#endif

#include "QApplication"
#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaEnum>
#include <QDir>
#include <QThreadPool>
#include <QRegularExpression>

#include "PythonQt.h"
#include "PythonQtObjectPtr.h"
#include "PythonQtMethodInfo.h"
#include "PythonQtConversion.h"

#include "gt_abstractproperty.h"
#include "gt_project.h"
#include "gt_task.h"
#include "gt_calculator.h"
#include "gt_coreapplication.h"
#include "gt_calculatordata.h"
#include "gt_calculatorfactory.h"
#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
#include "gt_datazone0d.h"
#endif

#include "gtpy_globals.h"
#include "gtpy_code.h"
#include "gtpy_stdout.h"
#include "gtpy_decorator.h"
#include "gtpy_interruptrunnable.h"
#include "gtpy_scriptrunnable.h"
#include "gtpy_loggingmodule.h"
#include "gtpy_extendedwrapper.h"
#include "gtpy_createhelperfunction.h"
#include "gtpy_propertysetter.h"
#include "gtpy_importfunction.h"
#include "gtpy_calculatorsmodule.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gtpy_matplotlib.h"
#endif

#include "gtpy_contextmanager.h"

#include "gtpypp.h"

namespace
{

GtpyContext::ContextType contextTypeEnumConvert(
        GtpyContextManager::Context type)
{
    switch (type)
    {
    case GtpyContextManager::BatchContext:
        return GtpyContext::BatchContext;

    case GtpyContextManager::GlobalContext:
        return GtpyContext::GlobalContext;

    case GtpyContextManager::ScriptEditorContext:
        return GtpyContext::ScriptEditorContext;

    case GtpyContextManager::CalculatorRunContext:
        return GtpyContext::CalculatorRunContext;

    case GtpyContextManager::TaskEditorContext:
        return GtpyContext::TaskEditorContext;

    case GtpyContextManager::TaskRunContext:
        return GtpyContext::TaskRunContext;

    case GtpyContextManager::CollectionContext:
        return GtpyContext::DefaultContext;
    }

    assert(false);
    return static_cast<GtpyContext::ContextType>(-1);
}

GtpyContext::InputType evalOptEnumConvert(GtpyContextManager::EvalOptions opt)
{
    switch (opt)
    {
    case GtpyContextManager::EvalFile:
        return GtpyContext::Script;

    case GtpyContextManager::EvalSingleString:
        return GtpyContext::SingleStatement;
    }

    assert(false);
    return static_cast<GtpyContext::InputType>(-1);
}

}

///critical static strings
const QString GtpyContextManager::LIST_DATATYPE = QStringLiteral("QList");
const QString GtpyContextManager::VARIANT_DATATYPE =
    QStringLiteral("QVariant");

const QString GtpyContextManager::FUNCTION_WARNING =
    QStringLiteral("WARNING: function not found");

const QString GtpyContextManager::CLASS_WRAPPER_MODULE =
    QStringLiteral("GtClasses");

GtpyContextManager::GtpyContextManager(QObject* parent) :
    QObject(parent), m_decorator(nullptr),
    m_errorEmitted(false),
    m_pyThreadState(nullptr),
    m_contextsInitialized(false)
{
    qRegisterMetaType<GtpyContextManager::Context>
        ("GtpyContextManager::Context");

#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
    setEnvironmentPaths();
#endif

#ifdef Q_OS_LINUX
    dlopen(PYTHON_LIBRARY, RTLD_LAZY | RTLD_GLOBAL);
#endif

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
    PythonQt::self()->registerClass(&GtCalculator::staticMetaObject,
                                    CLASS_WRAPPER_MODULE.toLocal8Bit().data());
#if GT_VERSION < 0x020000
    PythonQt::self()->registerClass(&GtDataZone0D::staticMetaObject,
                                    CLASS_WRAPPER_MODULE.toLocal8Bit().data());
#endif
    registerTypeConverters();

    m_decorator = new GtpyDecorator(this);

    PythonQt::self()->addDecorators(m_decorator);

    PythonQt::self()->registerCPPClass(
        "GtpyProcessDataDistributor", 0,
        CLASS_WRAPPER_MODULE.toLocal8Bit().data());

    connect(m_decorator, SIGNAL(sendErrorMessage(QString)), this,
            SLOT(onErrorMessage(QString)));

    connect(PythonQt::self(), SIGNAL(systemExitExceptionRaised(int)), this,
            SLOT(onSystemExitExceptionRaised(int)));

    m_pyThreadState = PyEval_SaveThread();

    GtpyCustomization::customizeSlotCalling();
}

GtpyContextManager*
GtpyContextManager::instance()
{
    static GtpyContextManager mgr(nullptr);
    return &mgr;
}

GtpyContextManager::~GtpyContextManager()
{
    if (m_pyThreadState != nullptr)
    {
        PyEval_RestoreThread(m_pyThreadState);
    }

    m_contextMap.clear();

    PythonQt::cleanup();
}

QString
GtpyContextManager::collectionPath()
{
    QString retval;

    QDir dir(qApp->applicationDirPath());

    if (dir.cdUp())
    {
        dir.setPath(dir.absolutePath() + QDir::separator() + "Collections" +
                    QDir::separator() + gtpy::constants::COLLECTION_ID);

        retval = dir.absolutePath();
    }

    return retval;
}

#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
void
GtpyContextManager::setEnvironmentPaths() const
{
    QString var = qEnvironmentVariable("PYTHONHOME");

    if (var.isEmpty())
    {
        return;
    }

    QDir pythonDir(var);

    if (!pythonDir.exists())
    {
        return;
    }

    QString paths = qEnvironmentVariable("PATH");

    if (!paths.isEmpty())
    {
        if (!paths.endsWith(";"))
        {
            paths.append(";");
        }
    }

    paths += QDir::toNativeSeparators(pythonDir.path()) + ";";

    if (pythonDir.cd("Library"))
    {
        paths += QDir::toNativeSeparators(pythonDir.path()) + ";";
    }

    if (pythonDir.cd("bin"))
    {
        paths += QDir::toNativeSeparators(pythonDir.path()) + ";";
    }

    qputenv("PATH", paths.toUtf8());
}
#endif

void
GtpyContextManager::initExtensions()
{
    GTPY_GIL_SCOPE

    if (PyType_Ready(&GtpyStdOutRedirect_Type) < 0)
    {
        gtError() << "could not initialize GtpyStdOutRedirect_Type";
    }

    Py_INCREF(&GtpyStdOutRedirect_Type);

    if (PyType_Ready(&GtpyExtendedWrapperModule::GtpyExtendedWrapper_Type) < 0)
    {
        gtError() << "could not initialize GtpyExtendedWrapper_Type";
    }

    Py_INCREF(&GtpyExtendedWrapperModule::GtpyExtendedWrapper_Type);

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

    if (PyType_Ready(&GtpyCalculatorsModule::GtpyCreateCalculator_Type) < 0)
    {
        gtError() << "could not initialize GtpyCreateCalculator_Type";
    }

    Py_INCREF(&GtpyCalculatorsModule::GtpyCreateCalculator_Type);

    if (PyType_Ready(&GtpyLoggingModule::GtpyPyLogger_Type) < 0)
    {
        gtError() << "could not initialize GtpyPyLogger_Type";
    }

    Py_INCREF(&GtpyLoggingModule::GtpyPyLogger_Type);

    initLoggingModuleC();
    initWrapperModule();
    initCalculatorsModule();
    initImportBehaviour();

    addCollectionPaths();
}

bool
GtpyContextManager::evalScript(int contextId,
                               const QString& script,
                               const bool output,
                               const bool errorMessage,
                               const EvalOptions& option)
{
    assert(m_contextsInitialized);

    GTPY_GIL_SCOPE

    setMetaDataToThreadDict(contextId, output, errorMessage);

    if (output)
    {
        emit startedScriptEvaluation(contextId);
    }

    auto con = context(contextId);
    if (!con) return false;

    bool success = true;

    if (!script.isEmpty())
    {
        success = con->eval(script, evalOptEnumConvert(option));
    }

    if (output || (!success && errorMessage))
    {
        emit scriptEvaluated(contextId);
    }

    return success;
}

QMultiMap<QString, GtpyFunction>
GtpyContextManager::introspection(int contextId, const QString& objectname,
                                  const bool appendModules)
{
    GTPY_GIL_SCOPE

    auto con = context(contextId);
    if (!con) return {};

    QMultiMap<QString, GtpyFunction> results;

    const bool objNameIsEmpty = objectname.isEmpty();

    static bool standardCompletionsSet = false;

    if (!standardCompletionsSet)
    {
        setStandardCompletions(contextId);
        setImportableModulesCompletions(contextId);
        standardCompletionsSet = true;
    }

    PyPPObject object;

    if (objNameIsEmpty)
    {
        object = con->module();
    }
    else
    {
        object = PyPPObject::Borrow(
            PythonQt::self()->lookupObject(con->module().get(), objectname));
    }

    if (object)
    {
        results = introspectObject(object.get());
    }
    else
    {
        QString temp = QStringLiteral("obj");
        QString script = temp + QStringLiteral(" = ") + objectname;

        bool eval = evalScript(contextId, script, false, false,
                               EvalSingleString);

        if (eval)
        {
            object = PyPPObject::Borrow(
                PythonQt::self()->lookupObject(con->module().get(), temp));

            if (object)
            {
                results = introspectObject(object.get());
            }

            con->removeVariable(temp);
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

    if (obj == nullptr)
    {
        return false;
    }

    GTPY_GIL_SCOPE

    auto con = context(contextId);
    if (!con) return false;

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

    con->addObject(name, obj);

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

    if (obj == nullptr)
    {
        return false;
    }

    GTPY_GIL_SCOPE

    auto con = context(contextId);
    if (!con) return false;

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

    auto argsTuple = PyPPTuple_New(1);

    auto pyQtWrapper = PyPPObject::fromQObject(obj);

    if (pyQtWrapper && (pyQtWrapper->ob_type->tp_base ==
                        &PythonQtInstanceWrapper_Type))
    {
        PyPPTuple_SetItem(argsTuple, 0, std::move(pyQtWrapper));
    }


    auto wrapped = PyPPObject::NewRef(
        GtpyExtendedWrapperModule::GtpyExtendedWrapper_Type.tp_new(
            &GtpyExtendedWrapperModule::GtpyExtendedWrapper_Type, argsTuple.get(),
            NULL));

    auto mod = con->module();
    PyPPModule_AddObject(mod, name.toLatin1().data(), std::move(wrapped));

    return true;
}

bool
GtpyContextManager::addVariable(int contextId, const QString& name, const QVariant& value)
{
    GTPY_GIL_SCOPE

    auto con = context(contextId);
    if (!con) return false;

    con->addVariable(name, value);

    return true;
}

QVariant
GtpyContextManager::getVariable(int contextId, const QString &name)
{
    GTPY_GIL_SCOPE

    auto con = context(contextId);
    if (!con) return {};

    auto obj = PyPPObject::Borrow(
        PythonQt::self()->lookupObject(con->module().get(), name));
    return PyPPObject_AsQVariant(obj);
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

    auto con = context(contextId);
    if (!con) return false;

    con->removeVariable(name);

    list.removeOne(name);

    m_addedObjectNames.insert(contextId, list);

    return true;
}

bool
GtpyContextManager::removeAllAddedObjects(int contextId)
{
    GTPY_GIL_SCOPE

    auto con = context(contextId);
    if (!con) return false;

    QStringList list = m_addedObjectNames.value(contextId, QStringList());

    foreach (QString objName, list)
    {
        con->removeVariable(objName);
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

    if (task != nullptr)
    {
        addObject(contextId, gtpy::code::attrs::TASK, task, false);

    }
    else
    {        
        auto pyCode = QStringLiteral("%1 = None").arg(gtpy::code::attrs::TASK);

        evalScript(contextId, pyCode, false);
    }

    return true;
}

void
GtpyContextManager::deleteCalcsFromTask(int contextId)
{
    if (!m_calcAccessibleContexts.contains(contextId))
    {
        return;
    }

    auto pyCode = QStringLiteral("%1.deleteAllCalculators()")
                      .arg(gtpy::code::attrs::TASK);

    evalScript(contextId, pyCode, false);
}

QString
GtpyContextManager::qvariantToPyStr(const QVariant& val) const
{
    GTPY_GIL_SCOPE

    return PyPPObject_AsQString(PyPPObject::fromQVariant(val));
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
    if (m_contextsInitialized) return;

    initExtensions();

    QMetaObject metaObj = GtpyContextManager::staticMetaObject;
    QMetaEnum metaEnum = metaObj.enumerator(
        metaObj.indexOfEnumerator("Context"));

    int keyCount = metaEnum.keyCount();

    for (int i = 0;  i < keyCount; i++)
    {
        auto type = contextTypeEnumConvert(
            static_cast<Context>(metaEnum.value(i)));

        int contextId = metaEnum.value(i);

        m_contextMap.insert(contextId, std::make_shared<GtpyContext>(type));

        if (type == GtpyContext::TaskEditorContext ||
            type == GtpyContext::TaskRunContext)
        {
            if (!m_calcAccessibleContexts.contains(contextId))
            {
                m_calcAccessibleContexts << contextId;
            }
        }
    }

    initStdOut();

    m_contextsInitialized = true;
}

int
GtpyContextManager::createNewContext(const GtpyContextManager::Context& type,
                                     bool emitSignal)
{
    auto keys = m_contextMap.keys();

    // We let user contexts start from 100 to have room for system contexts
    int contextId = keys.isEmpty() ? 100 :
                        *std::max_element(keys.begin(), keys.end()) + 1;
    auto contextType = contextTypeEnumConvert(type);

    m_contextMap.insert(contextId, std::make_shared<GtpyContext>(contextType));

    if ((contextType == GtpyContext::TaskEditorContext ||
         contextType == GtpyContext::TaskRunContext ) &&
        !m_calcAccessibleContexts.contains(contextId))
    {
        m_calcAccessibleContexts << contextId;
    }

    m_addedObjectNames.insert(contextId, {});

    if (emitSignal)
    {
        emit newContextCreated(contextId);
    }

    return contextId;
}

bool
GtpyContextManager::deleteContext(int contextId, bool emitSignal)
{
    m_contextMap.take(contextId);

    if (emitSignal)
    {
        emit contextDeleted(contextId);
    }

    m_calcAccessibleContexts.removeOne(contextId);

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

    auto contextType = contextTypeEnumConvert(type);

    m_contextMap.insert(contextId, std::make_shared<GtpyContext>(contextType));

    if (contextType == GtpyContext::TaskEditorContext ||
        contextType == GtpyContext::TaskRunContext)
    {
        if (!m_calcAccessibleContexts.contains(contextId))
        {
            m_calcAccessibleContexts << contextId;
        }
    }
    else
    {
        m_calcAccessibleContexts.removeOne(contextId);
    }
}

PythonQtObjectPtr
GtpyContextManager::contextPointer(int contextId) const
{
    auto con = context(contextId);
    if (!con) return {};

    return PythonQtObjectPtr{con->module().get()};
}

long
GtpyContextManager::currentPyThreadId()
{
    GTPY_GIL_SCOPE

    return PyThreadState_Get()->thread_id;
}

void
GtpyContextManager::autoDeleteRunnable(GtpyScriptRunnable* runnable)
{
    if (runnable && !runnable->autoDelete())
    {
        connect(runnable, SIGNAL(runnableFinished()), this,
                SLOT(deleteRunnable()));
    }
}

void
GtpyContextManager::interruptPyThread(long id)
{
    GtpyInterruptRunnable* runnable = new GtpyInterruptRunnable(id);

    QThreadPool* tp = QThreadPool::globalInstance();

    // start runnable
    tp->start(runnable);
}

StdOutMetaData
GtpyContextManager::threadDictMetaData()
{
    GTPY_GIL_SCOPE

    auto threadDict = PyPPThreadState_GetDict();

    if (!threadDict)
    {
        return {};
    }

    StdOutMetaData retval;

    auto valItem = PyPPDict_GetItem(threadDict, gtpy::code::keys::CONTEXT);

    if (valItem && PyPPUnicode_Check(valItem))
    {
        retval.contextName = PyPPString_AsQString(valItem);
    }

    auto outputItem = PyPPDict_GetItem(threadDict, gtpy::code::keys::OUTPUT);

    if (outputItem && PyPPLong_Check(outputItem))
    {
        retval.output = (bool)PyPPLong_AsLong(outputItem);
    }

    auto errorItem = PyPPDict_GetItem(threadDict, gtpy::code::keys::ERROR);

    if (errorItem && PyPPLong_Check(errorItem))
    {
        retval.error = (bool)PyPPLong_AsLong(errorItem);
    }

    return retval;
}

bool
GtpyContextManager::createCustomModule(
    const QString& moduleName, const QString& code)
{
    GTPY_GIL_SCOPE

    auto module = PythonQt::self()->createModuleFromScript(moduleName);

    if (!module) return false;

    // We need to delete this attribute, as the new module is built-in,see
    // https://docs.python.org/3/reference/import.html#file__
    // also see issue #284
    PyObject_DelAttrString(module, "__file__");

    module.evalScript(code);

    return !PythonQt::self()->hadError();
}

std::shared_ptr<GtpyContext>
GtpyContextManager::context(int contextId) const
{
    return m_contextMap.value(contextId, nullptr);
}

#ifdef PY3K
PyPPObject
GtpyContextManager::initExtensionModule(const QString& moduleName,
                                        PyModuleDef* def)
{
    if (!def)
    {
        return {};
    }

    GTPY_GIL_SCOPE

    QByteArray name = moduleName.toUtf8();

    auto myMod = PyPPModule_Create(def);

    modulenameToBuiltins(moduleName);

    auto sys = PyPPImport_ImportModule("sys");

    if (!sys)
    {
        return {};
    }

    auto modules = PyPPObject_GetAttr(sys, "modules");
    PyPPDict_SetItem(modules, name.constData(), myMod);

    return myMod;
}
#else
PyPPObject
GtpyContextManager::initExtensionModule(const QString& moduleName,
                                        PyMethodDef* methods)
{
    GTPY_GIL_SCOPE

            QByteArray name = moduleName.toUtf8();
    PyObject* myMod = nullptr;

    myMod = Py_InitModule(name.constData(), methods);
    Py_INCREF(myMod);

    modulenameToBuiltins(moduleName);

    return PyPPObject::NewRef(myMod);
}
#endif

void
GtpyContextManager::modulenameToBuiltins(QString name)
{
    GTPY_GIL_SCOPE

    auto sys = PyPPImport_ImportModule("sys");

    if (!sys)
    {
        return;
    }

    // add the new module to the list of builtin module names
    const auto old_module_names =
        PyPPObject_GetAttr(sys, "builtin_module_names");

    if (!old_module_names || !PyPPTuple_Check(old_module_names)) return;


    Py_ssize_t old_size = PyPPTuple_Size(old_module_names);
    auto module_names = PyPPTuple_New(old_size + 1);

    for (Py_ssize_t i = 0; i < old_size; i++)
    {
        auto item = PyPPTuple_GetItem(old_module_names, i);
        if (item)
        {
            PyPPTuple_SetItem(module_names, i, std::move(item));
        }
    }

    PyPPTuple_SetItem(module_names, old_size, PyPPObject::fromQString(name));
    PyPPModule_AddObject(sys, "builtin_module_names", std::move(module_names));
}

void
GtpyContextManager::initCalculatorsModule()
{
    GTPY_GIL_SCOPE
#ifdef PY3K
    initExtensionModule(gtpy::code::modules::GT_CALCULATORS,
                        &GtpyCalculatorsModule::GtpyCalculators_Module);
#else
    initExtensionModule(gtpy::code::modules::GT_CALCULATORS,
                        GtpyCalculatorsModule::GtpyCalculatorsModule_StaticMethods);
#endif
    GtpyCalculatorsModule::createCalcConstructors();
}

void
GtpyContextManager::initLoggingModuleC()
{
    GTPY_GIL_SCOPE

#ifdef PY3K
    initExtensionModule(gtpy::code::modules::GT_LOGGING,
                        &GtpyLoggingModule::GtpyLogging_Module);
#else
    initExtensionModule(gtpy::code::modules::GT_LOGGING,
                        GtpyLoggingModule::GtpyLoggingModule_StaticMethods);
#endif
}

void
GtpyContextManager::initImportBehaviour()
{
    GTPY_GIL_SCOPE

    auto builtins = PyPPImport_ImportModule("builtins");

    if (!builtins)
    {
        builtins = PyPPImport_ImportModule("__builtin__");
    }

    if (!builtins || !PyPPModule_Check(builtins))
    {
        return;
    }

    auto dict = PyPPModule_GetDict(builtins);
    if (!dict) return;

    auto defImp = PyPPDict_GetItem(dict, "__import__");
    if (!defImp) return;

    auto imp = PyPPObject::NewRef(GtpyMyImport_Type.tp_new(
        &GtpyMyImport_Type, NULL, NULL));

    // transfer ownership to imp
    ((GtpyMyImport*)imp.get())->defaultImp = defImp.release();
    PyPPDict_SetItem(dict, "__import__", std::move(imp));
}

void
GtpyContextManager::initStdOut()
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr sys;
    sys.setNewRef(PyImport_ImportModule("sys"));

    // create a redirection object for stdout and stderr
    m_out = GtpyStdOutRedirect_Type.tp_new(&GtpyStdOutRedirect_Type, NULL,
                                           NULL);
    ((GtpyStdOutRedirect*)m_out.object())->callback = stdOutRedirectCB;

    m_err = GtpyStdOutRedirect_Type.tp_new(&GtpyStdOutRedirect_Type, NULL,
                                           NULL);
    ((GtpyStdOutRedirect*)m_err.object())->callback = stdErrRedirectCB;

    // replace the built in file objects with the new objects
    PyModule_AddObject(sys, "stdout", m_out);
    PyModule_AddObject(sys, "stderr", m_err);
}

void
GtpyContextManager::initWrapperModule()
{
    GTPY_GIL_SCOPE

#ifdef PY3K
    auto mod = initExtensionModule(
        gtpy::code::modules::GT_OBJECT_WRAPPER_MODULE,
        &GtpyExtendedWrapperModule::GtpyExtendedWrapper_Module);
#else
    auto mod = initExtensionModule(
        gtpy::code::modules::GT_OBJECT_WRAPPER_MODULE, nullptr);
#endif

    auto wrapperType = PyPPObject::NewRef(
        (PyObject*)&GtpyExtendedWrapperModule::GtpyExtendedWrapper_Type);

    PyPPModule_AddObject(mod, gtpy::code::classes::GTPY_EXTENDED_WRAPPER,
                         std::move(wrapperType));
}

void
GtpyContextManager::addCollectionPaths()
{
    QString path = collectionPath();

    QDir collectionDir(path);

    m_watcher.addPath(collectionDir.absolutePath());
    connect(&m_watcher, SIGNAL(directoryChanged(QString)), this,
            SLOT(collectionChanged(QString)));

    if (collectionDir.exists())
    {
        collectionDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot |
                                QDir::NoSymLinks);

        QStringList subdirs = collectionDir.entryList();

        foreach (QString subdir, subdirs)
        {
            QString nativePath = QDir::toNativeSeparators(
                collectionDir.absolutePath());
            nativePath = nativePath + QDir::separator() + subdir;

            addModulePath(nativePath);
        }
    }
}

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

    int errorLineNumber = match.captured().mid(17).toInt(&ok);

    if (ok)
    {
        return errorLineNumber;
    }
    else
    {
        return -1;
    }
}

void
GtpyContextManager::setMetaDataToThreadDict(const StdOutMetaData& mData)
{
    GTPY_GIL_SCOPE

    auto threadDict = PyPPThreadState_GetDict();

    if (!threadDict)
    {
        return;
    }

    {
        const auto key = PyPPObject::fromQString(mData.contextName);
        PyPPDict_SetItem(threadDict, gtpy::code::keys::CONTEXT, key);
    }

    {
        const auto outputVal = PyPPObject::fromLong(mData.output ? 1 : 0);
        PyPPDict_SetItem(threadDict, gtpy::code::keys::OUTPUT, outputVal);
    }

    {
        const auto errorVal = PyPPObject::fromLong(mData.error ? 1 : 0);
        PyPPDict_SetItem(threadDict, gtpy::code::keys::ERROR, errorVal);
    }

}

void
GtpyContextManager::addModulePaths(const QStringList& paths)
{
    foreach (QString path, paths)
    {
        addModulePath(path);
    }
}

void
GtpyContextManager::addModulePath(const QString& path)
{
    GTPY_GIL_SCOPE

        auto pyPath = PyPPSys_GetObject("path");

    if (!pyPath) return;

    const auto pathObj = PyPPObject::fromQString(path);
    bool equal = false;

    for (int i = 0; !equal && i < Py_SIZE(pyPath.get()); i++)
    {
        const auto item = PyPPList_GetItem(pyPath, i);
        equal = PyPPObject_RichCompareBool(item, pathObj, Py_EQ);
    }

    if (!equal)
    {
        PyPPList_Append(pyPath, pathObj);
    }
}

bool GtpyContextManager::initMatplotlib()
{
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    gtDebug() << "Initializing Matplotlib backend";
    if (!createCustomModule(gtpy::matplotlib::backendName,
                            gtpy::matplotlib::customBackend)) return false;

    if (!evalScript(GtpyContextManager::GlobalContext,
                    gtpy::matplotlib::setCustomBackend)) return false;
#endif

    return true;
}

void
GtpyContextManager::setMetaDataToThreadDict(int contextId, bool output,
                                            bool error)
{
    StdOutMetaData metaData;

    metaData.contextName = contextNameById(contextId);
    metaData.output = output;
    metaData.error = error;

    setMetaDataToThreadDict(metaData);
}

QMultiMap<QString, GtpyFunction>
GtpyContextManager::introspectObject(PyObject* objectIn) const
{
    GTPY_GIL_SCOPE

            QMultiMap<QString, GtpyFunction> results;

    if (m_decorator == nullptr)
    {
        return results;
    }

    if (!objectIn)
    {
        return results;
    }

    auto object = PyPPObject::Borrow(objectIn);


    PyPPObject keys;
    bool isDict = false;

    if (PyPPDict_Check(object))
    {
        keys = PyPPDict_Keys(object);
        isDict = true;
    }
    else
    {
        keys = PyPPObject_Dir(object);
    }

    if (keys)
    {
        int count = PyPPList_Size(keys);

        for (int i = 0; i < count; i++)
        {
            auto key = PyPPList_GetItem(keys, i);

            if (!key)
            {
                continue;
            }

            PyPPObject value = isDict ?
                                   PyPPDict_GetItem(object, key) :
                                   PyPPObject_GetAttr(object, key);

            if (!value) continue;

            const auto keystr = PyPPString_AsQString(key);

            static const QString underscoreStr(QStringLiteral("__"));

            if (!keystr.startsWith(underscoreStr))
            {
                GtpyFunction func;

                if (value->ob_type == &PythonQtSlotFunction_Type)
                {
                    func.name = keystr + QStringLiteral("()");

                    PythonQtSlotInfo* info =
                        ((PythonQtSlotFunctionObject*)value.get())->m_ml;

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
                         value->ob_type == &PyMethod_Type ||
                         PyPPCallable_Check(value))
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

        }

    }

    PythonQtObjectPtr p = object.get();

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

                if (obj != nullptr && !obj->objectName().isEmpty())
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
                        static QRegularExpression regexp{
                                                         "^[a-zA-Z_][a-zA-Z0-9_]*$"};

                        if (!objName.contains(regexp))
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

                if (prop != nullptr && !prop->objectName().isEmpty())
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
    printFunc.completion = print + QStringLiteral("()");
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

    auto con = context(contextId);
    if (!con) return results;

    con->eval(QStringLiteral("import inspect"));
    con->eval(QStringLiteral("def __builtins():") +
                   QStringLiteral("return dir(inspect.builtins)"));

    auto tmpPythonQtPtr = PythonQtObjectPtr{con->module().get()};
    QVariant v = tmpPythonQtPtr.call(QStringLiteral("__builtins"));

    con->eval(QStringLiteral("del inspect"));

    if (v.isNull())
    {
        return results;
    }

    QStringList builtinFunctions = v.toStringList();

    foreach (QString name, builtinFunctions)
    {
        if (name.isEmpty() || name.startsWith("_"))
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

    auto con = context(contextId);
    if (!con) return;

    con->eval(QStringLiteral("import pkgutil"));
    con->eval(QStringLiteral("def __importableModules():\n") +
                   QStringLiteral("\tmodules = pkgutil.iter_modules()\n") +
                   QStringLiteral("\tx = (i.name for i in modules)\n") +
                   QStringLiteral("\treturn list(x)"));

    auto tmpPythonQtPtr = PythonQtObjectPtr{con->module().get()};
    QVariant v = tmpPythonQtPtr.call(QStringLiteral("__importableModules"));

    con->eval(QStringLiteral("del pkgutil"));

    if (v.isNull())
    {
        return;
    }

    QStringList modules = v.toStringList();

    modules.append(QStringLiteral("sys"));  // sys is somehow not listed
    modules.append(QStringLiteral("time")); // time is somehow not listed

    foreach (QString name, modules)
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
        if (!gtDevMode() &&
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

    objectPtrMapId = qRegisterMetaType<QMap<QString, QString>>(
        "QMap<QString, QString>");

    PythonQtConv::registerMetaTypeToPythonConverter(
        objectPtrMapId, GtpyTypeConversion::convertFromQMapStringQString);
    PythonQtConv::registerPythonToMetaTypeConverter(
        objectPtrMapId, GtpyTypeConversion::convertToQMapStringQString);
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
        auto con = context(id);
        assert(con);

        if (con->moduleName() == contextName)
        {
            return id;
        }
    }

    return -1;
}

QString
GtpyContextManager::contextNameById(int contextId)
{

    auto con = context(contextId);
    if (!con) return {};

    return con->moduleName();
}

void
GtpyContextManager::onErrorMessage(const QString& message)
{
    GTPY_GIL_SCOPE

    const auto threadDict = PyPPThreadState_GetDict();

    if (!threadDict)
    {
        return;
    }

    auto item = PyPPDict_GetItem(threadDict, gtpy::code::keys::CONTEXT);

    int contextId = -1;

    if (item)
    {
        contextId = contextIdByName(PyPPString_AsQString(item));
    }

    item = PyPPDict_GetItem(threadDict, gtpy::code::keys::ERROR);

    bool error = false;

    if (item && PyPPLong_Check(item))
    {
        error = (bool)PyPPLong_AsLong(item);
    }

    if (error && contextId > -1)
    {
        emit errorMessage(message, contextId);
    }
}

void
GtpyContextManager::onSystemExitExceptionRaised(const int /*exep*/) const
{
    //Has to exist to keep GTlab running when a python script calls sys.exit()
}

void
GtpyContextManager::deleteRunnable()
{
    GtpyScriptRunnable* runnable = qobject_cast<GtpyScriptRunnable*>(sender());

    if (runnable != nullptr)
    {
        // connect runnable signals to task runner slots
        disconnect(runnable, &GtpyScriptRunnable::runnableFinished,
                   this, &GtpyContextManager::deleteRunnable);

        delete runnable;
    }
}

void
GtpyContextManager::collectionChanged(const QString& collectionPath)
{
    QDir dir(collectionPath);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList subdirs = dir.entryList();

    foreach (QString subdir, subdirs)
    {
        QString nativePath = QDir::toNativeSeparators(dir.absolutePath());
        nativePath = nativePath + QDir::separator() + subdir;

        addModulePath(nativePath);
    }
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

PyObject *
GtpyTypeConversion::convertFromQMapStringQString(const void *inObject, int)
{
    return mapToPython<QString, QString>(inObject);
}

bool
GtpyTypeConversion::convertToQMapStringQString(PyObject *obj, void *outMap,
                                               int, bool)
{
    return pythonToMap<QString, QString>(obj, outMap);
}
