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

#include "gtpy_calculatorfactory.h"
#include "gtpy_gilscope.h"
#include "gtpy_decorator.h"
#include "gtpy_interruptrunnable.h"
#include "gtpy_stdout.h"

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
    QStringLiteral("CalculatorFactory");
const QString GtpyContextManager::HELPER_FAC_VAR =
    QStringLiteral("HelperFactory");

const QString GtpyContextManager::CLASS_WRAPPER_MODULE =
    QStringLiteral("GtClasses");
const QString GtpyContextManager::LOGGING_MODULE =
    QStringLiteral("GtLogging");
const QString GtpyContextManager::CALC_MODULE =
    QStringLiteral("GtCalculators");

GtpyContextManager::GtpyContextManager(QObject* parent) :
    QObject(parent), m_decorator(Q_NULLPTR),
    m_errorEmitted(false),
    m_loggingModule(Q_NULLPTR),
    m_pyThreadState(Q_NULLPTR)
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

    connect(m_decorator, SIGNAL(sendErrorMessage(QString)), this,
            SLOT(onErrorMessage(QString)));

    connect(PythonQt::self(), SIGNAL(systemExitExceptionRaised(int)), this,
            SLOT(onSystemExitExceptionRaised(int)));

    m_pyThreadState = PyEval_SaveThread();
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
GtpyContextManager::evalScript(const GtpyContextManager::Context& type,
                               const QString& script,
                               const bool output,
                               const bool errorMessage,
                               const EvalOptions& option)
{
    GTPY_GIL_SCOPE

    setMetaDataToThreadDict(type, output, errorMessage);

    if (output)
    {
        emit startedScriptEvaluation(type);
    }

    PythonQtObjectPtr currentContext = context(type);

    if (currentContext == Q_NULLPTR)
    {
        return false;
    }

    if (m_appLogging.value(type, false))
    {
        m_loggingModule.evalScript(QStringLiteral(
                                 "PyLogger._PyLogger__outputToConsole = True"));
    }
    else
    {
        m_loggingModule.evalScript(QStringLiteral(
                                "PyLogger._PyLogger__outputToConsole = False"));
    }

    if (m_calcAccessibleContexts.contains(type))
    {
        QString pyCode =
                "if '" + CALC_MODULE + "' in locals():\n" +
                "    import sys\n" +
                "    sys.modules['" + CALC_MODULE + "'] = " + CALC_MODULE +
                "\n" +
                "    del sys\n";

        currentContext.evalScript(pyCode);
    }

    bool hadError = false;

    if (!script.isEmpty())
    {
        currentContext.evalScript(script, option);
        hadError = PythonQt::self()->hadError();
    }

    if (output || (hadError && errorMessage))
    {
        emit scriptEvaluated(type);
    }

    if (m_calcAccessibleContexts.contains(type))
    {
        removeCalcModuleFromSys();
    }

    return !hadError;
}

bool
GtpyContextManager::evalScriptInterruptible(
        const GtpyContextManager::Context& type, const QString& script,
        const bool output, const GtpyContextManager::EvalOptions& option)
{
    GTPY_GIL_SCOPE

    bool success = false;

    success = evalScript(type, script, output, option);

//    interruptibleCode.replace("\r", "\n");

//    interruptibleCode.prepend("try:\n");
//    interruptibleCode = interruptibleCode.replace("\n","\n\t");
//    interruptibleCode += "\nexcept KeyboardInterrupt:\n"
//                         "\tprint ('--Interrupted--')\n";

    return success;
}

QMultiMap<QString, GtpyFunction>
GtpyContextManager::introspection(const GtpyContextManager::Context& type,
                                  const QString& objectname,
                                  const bool appendModules)
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr currentContext = context(type);

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
        setStandardCompletions(type);
        setImportableModulesCompletions(type);
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

        bool eval = evalScript(type, script, false, false, EvalSingleString);

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
        results = results + calculatorCompletions(type) +
                  m_standardCompletions;

        if (appendModules)
        {
            results += m_importableModulesCompletions;
        }
    }

    return results;
}

bool
GtpyContextManager::addObject(const GtpyContextManager::Context& type,
                              const QString& name, QObject* obj,
                              const bool saveName)
{
    GTPY_GIL_SCOPE

    if (name.isEmpty())
    {
        return false;
    }

    if (obj == Q_NULLPTR)
    {
        return false;
    }

    PythonQtObjectPtr currentContext = context(type);

    if (currentContext == Q_NULLPTR)
    {
        return false;
    }

    QStringList list = m_addedObjectNames.value(type, QStringList());

    if (list.contains(name))
    {
        return false;
    }

    if (saveName)
    {
        list.append(name);
        m_addedObjectNames.insert(type, list);
    }

    currentContext.addObject(name, obj);

    return true;
}

bool
GtpyContextManager::removeObject(const GtpyContextManager::Context&
                                     type, const QString& name)
{
    GTPY_GIL_SCOPE

    QStringList list = m_addedObjectNames.value(type, QStringList());

    if (!list.contains(name))
    {
        return false;
    }

    PythonQtObjectPtr currentContext = context(type);

    if (currentContext == Q_NULLPTR)
    {
        return false;
    }

    currentContext.removeVariable(name);

    list.removeOne(name);

    m_addedObjectNames.insert(type, list);

    return true;
}

bool
GtpyContextManager::removeAllAddedObjects(
    const GtpyContextManager::Context& type)
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr currentContext = context(type);

    if (currentContext == Q_NULLPTR)
    {
        return false;
    }

    QStringList list = m_addedObjectNames.value(type, QStringList());

    foreach (QString objName, list)
    {
        currentContext.removeVariable(objName);
    }

    m_addedObjectNames.insert(type, QStringList());

    return true;
}

bool
GtpyContextManager::addTaskValue(const GtpyContextManager::Context& type,
                                 GtTask* task)
{
    GTPY_GIL_SCOPE

    if (!m_calcAccessibleContexts.contains(type))
    {
        return false;
    }

    if (task != Q_NULLPTR)
    {
        addObject(type, TASK_VAR, task, false);

        QString pyCode =
            CALC_MODULE + QStringLiteral(".") + TASK_VAR +
                QStringLiteral(" = ") + TASK_VAR +  QStringLiteral("\n") +
            QStringLiteral("del ") + TASK_VAR + QStringLiteral("\n");

        evalScript(type, pyCode , false);
    }
    else
    {
        evalScript(type, CALC_MODULE + QStringLiteral(".") + TASK_VAR +
                   QStringLiteral(" = None"), false);
    }

    return true;
}

void
GtpyContextManager::deleteCalcsFromTask(
        const GtpyContextManager::Context& type)
{
    if (!m_calcAccessibleContexts.contains(type))
    {
        return;
    }

    evalScript(type, CALC_MODULE + QStringLiteral(".") + TASK_VAR
               + QStringLiteral(".deleteAllCalculators()"), false);
}

QString
GtpyContextManager::qvariantToPyStr(const QVariant& val) const
{
    GTPY_GIL_SCOPE

    PyObject* pyObj = PythonQtConv::QVariantToPyObject(val);

    return PythonQtConv::PyObjGetString(pyObj);
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
    initCalculatorModule();
    initLoggingModule();

    QMetaObject metaObj = GtpyContextManager::staticMetaObject;
    QMetaEnum metaEnum = metaObj.enumerator(
                             metaObj.indexOfEnumerator("Context"));

    int keyCount = metaEnum.keyCount();

    for (int i = 0;  i < keyCount; i++)
    {
        QString contextName = QString::fromUtf8(metaEnum.key(i));

        Context type = static_cast<Context>(metaEnum.value(i));

        defaultContextConfig(type, contextName);
    }

    initStdOut();

    removeCalcModuleFromSys();

    getContextNamesMap();
}

void
GtpyContextManager::resetContext(const GtpyContextManager::Context& type)
{
    GTPY_GIL_SCOPE

    if (m_calcAccessibleContexts.contains(type))
    {
        registerCalcModuleInSys(type);
    }

    QMetaObject metaObj = GtpyContextManager::staticMetaObject;
    QMetaEnum metaEnum = metaObj.enumerator(
                             metaObj.indexOfEnumerator("Context"));

    QString contextName = QString::fromUtf8(metaEnum.key(type));

    defaultContextConfig(type, contextName);

    if (m_calcAccessibleContexts.contains(type))
    {
        removeCalcModuleFromSys();
    }
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
        const GtpyContextManager::Context& type, const QString& contextName)
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr context = PythonQt::self()->createModuleFromScript(
                                    contextName);

    m_contextMap.insert(type, context);

    QString pyCode =
            "import sys\n"
            "sys.modules['" + contextName +
            "'] = None\n" +
            "del sys\n";

    context.evalScript(pyCode);

    m_addedObjectNames.insert(type, QStringList());

    specificContextConfig(type);
}

void
GtpyContextManager::specificContextConfig(
        const GtpyContextManager::Context& type)
{
    switch (type)
    {
        case GtpyContextManager::BatchContext:
            initBatchContext();
            break;
        case GtpyContextManager::GlobalContext:
            initGlobalContext();
            break;
        case GtpyContextManager::ScriptEditorContext:
            initScriptEditorContext();
            break;
        case GtpyContextManager::CalculatorRunContext:
            initCalculatorRunContext();
            break;
        case GtpyContextManager::TaskEditorContext:
            initTaskEditorContext();
            break;
        case GtpyContextManager::TaskRunContext:
            initTaskRunContext();
            break;
        default:
            break;
    }
}

PythonQtObjectPtr
GtpyContextManager::context(const GtpyContextManager::Context& type) const
{
    return m_contextMap.value(type, Q_NULLPTR);
}

void
GtpyContextManager::initCalculatorModule()
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr calcModule =
            PythonQt::self()->createModuleFromScript(CALC_MODULE);

    GtpyCalculatorFactory* factory = new GtpyCalculatorFactory(this);

    calcModule.evalScript(TASK_VAR + QStringLiteral(" = None"));

    calcModule.addObject(CALC_FAC_VAR, factory);

    calcModule.addObject(HELPER_FAC_VAR, gtCalculatorHelperFactory);

    calcModule.evalScript(

        QStringLiteral("class HelperWrapper: \n") +
        QStringLiteral("    def __init__(self, helper): \n") +
        QStringLiteral("        self._helper = helper\n") +

        QStringLiteral("        helpers = ") + HELPER_FAC_VAR + QStringLiteral(".connectedHelper(self._helper.__class__.__name__)\n") +
        QStringLiteral("        for i in range(len(helpers)):\n") +
        QStringLiteral("            funcName = 'create' + helpers[i]\n") +
        QStringLiteral("            def dynCreateHelper(name, self = self, helperName = helpers[i]):\n") +
        QStringLiteral("                helper = ") + HELPER_FAC_VAR + QStringLiteral(".newCalculatorHelper(helperName, name, self._helper)\n") +
        QStringLiteral("                return HelperWrapper(helper)\n") +
        QStringLiteral("            setattr(self, funcName, dynCreateHelper)\n") +

        QStringLiteral("    def __getattr__(self, name): \n") +
        QStringLiteral("        return getattr(self.__dict__['_helper'], name)\n\n") +
        QStringLiteral("    def __setattr__(self, name, value): \n") +
        QStringLiteral("        if name in ('_helper') or name.startswith('createGt'):\n") +
        QStringLiteral("            self.__dict__[name] = value\n") +
        QStringLiteral("        else:\n") +
        QStringLiteral("            setattr(self.__dict__['_helper'], name, value)\n") +
        QStringLiteral("    def __dir__(self): \n") +
        QStringLiteral("        return sorted(set(dir(type(self)) + dir(self._helper)))"));

    calcModule.evalScript(QStringLiteral("import re"));

    calcModule.evalScript(

        QStringLiteral("class CalcWrapper: \n") +
        QStringLiteral("    def __init__(self, calc):\n") +
        QStringLiteral("        self._calc = calc\n") +

        QStringLiteral("        for i in range(len(self._calc.findGtProperties())):\n") +
        QStringLiteral("            prop = self._calc.findGtProperties()[i]\n") +
        QStringLiteral("            if prop.ident():\n") +

        QStringLiteral("                funcName = re.sub('[^A-Za-z0-9]+', '', prop.ident())\n") +
        QStringLiteral("                tempLetter = funcName[0]\n") +
        QStringLiteral("                funcName = funcName.replace(tempLetter, tempLetter.lower(), 1)\n") +

        QStringLiteral("                def dynGetter(self = self, propName = prop.ident()):\n") +
        QStringLiteral("                    return self._calc.propertyValue(propName)\n") +
        QStringLiteral("                setattr(self, funcName, dynGetter)\n") +

        QStringLiteral("                tempLetter = funcName[0]\n") +
        QStringLiteral("                funcName = funcName.replace(tempLetter, tempLetter.upper(), 1)\n") +
        QStringLiteral("                funcName = 'set' + funcName\n") +

        QStringLiteral("                def dynSetter(val, self = self, propName = prop.ident()):\n") +
        QStringLiteral("                    self._calc.setPropertyValue(propName, val)\n") +
        QStringLiteral("                setattr(self, funcName, dynSetter)\n") +

        QStringLiteral("        helpers = ") + HELPER_FAC_VAR + QStringLiteral(".connectedHelper(self._calc.__class__.__name__)\n") +
        QStringLiteral("        for i in range(len(helpers)):\n") +
        QStringLiteral("            funcName = 'create' + helpers[i]\n") +

        QStringLiteral("            def dynCreateHelper(name, self = self, helperName = helpers[i]):\n") +
        QStringLiteral("                helper = ") + HELPER_FAC_VAR + QStringLiteral(".newCalculatorHelper(helperName, name, self._calc)\n") +
        QStringLiteral("                return HelperWrapper(helper)\n") +
        QStringLiteral("            setattr(self, funcName, dynCreateHelper)\n") +

        QStringLiteral("    def __getattr__(self, name): \n") +
        QStringLiteral("        return getattr(self.__dict__['_calc'], name)\n") +
        QStringLiteral("    def __setattr__(self, name, value): \n") +
        QStringLiteral("        if name is ('_calc') or  hasattr(self.__dict__['_calc'], name) is False:\n") +
        QStringLiteral("            self.__dict__[name] = value\n") +
        QStringLiteral("        else:\n") +
        QStringLiteral("            setattr(self.__dict__['_calc'], name, value)\n") +
        QStringLiteral("    def __dir__(self): \n") +
        QStringLiteral("        return sorted(list(self.__dict__.keys()) + dir(self._calc))\n"));


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

        calcModule.evalScript(

            QStringLiteral("def ") + className +
            QStringLiteral("(name = '") + calcData->id +
                   QStringLiteral("'):\n") +

            QStringLiteral("    tempCalc = ") + CALC_FAC_VAR +
                   QStringLiteral(".createCalculator(\"") +
            className + QStringLiteral("\", name, ") + TASK_VAR +
                   QStringLiteral(")\n") +

            QStringLiteral("    return CalcWrapper(tempCalc)"));
    }
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
GtpyContextManager::initBatchContext()
{
    GTPY_GIL_SCOPE

    Context type = BatchContext;
    PythonQtObjectPtr con = context(type);

    if (con == Q_NULLPTR)
    {
        return;
    }

    importDefaultModules(type);

    if (gtApp != Q_NULLPTR)
    {
        addObject(type, "GTlab", gtApp);

        QString pyCode =
            "def openProject(projectName):\n"
            "    return GTlab.openProject(projectName)\n"
            "def currentProject():\n"
            "    return GTlab.currentProject()\n"
            "def init(id = ''):\n"
            "    return GTlab.init(id)\n"
            "def switchSession(id = ''):\n"
            "    return GTlab.switchSession(id)\n";

        con.evalScript(pyCode);
    }
    else
    {
        qWarning() << tr("ERROR: GtApplication is not initialized!!! "
                         "Batch context can not register the GTlab object.");
    }

    importLoggingFuncs(type, true);
}

void
GtpyContextManager::initGlobalContext()
{
    GTPY_GIL_SCOPE

    Context type = GlobalContext;
    PythonQtObjectPtr con = context(type);

    if (con == Q_NULLPTR)
    {
        return;
    }

    importDefaultModules(type);

    if (gtApp != Q_NULLPTR)
    {
        addObject(type, QStringLiteral("GTlab"), gtApp);

        QString pyCode =
            "def openProject(projectName):\n"
            "    return GTlab.openProject(projectName)\n"
            "def currentProject():\n"
            "    return GTlab.currentProject()\n"
            "def init(id = ''):\n"
            "    return GTlab.init(id)\n"
            "def switchSession(id = ''):\n"
            "    return GTlab.switchSession(id)\n";

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

    con.evalScript(pyCode);

    importLoggingFuncs(type, true);
}

void
GtpyContextManager::initScriptEditorContext()
{
    Context type = ScriptEditorContext;

    importDefaultModules(type);

    importLoggingFuncs(type, false);
}

void
GtpyContextManager::initCalculatorRunContext()
{
    Context type = CalculatorRunContext;

    importDefaultModules(type);

    importLoggingFuncs(type, true);
}

void
GtpyContextManager::initTaskEditorContext()
{
    Context type = TaskEditorContext;

    importDefaultModules(type);

    importLoggingFuncs(type, false);

    importCalcModule(type);
}

void
GtpyContextManager::initTaskRunContext()
{
    Context type = TaskRunContext;

    importDefaultModules(type);

    importLoggingFuncs(type, true);

    importCalcModule(type);
}

void
GtpyContextManager::initStdOut()
{
    GTPY_GIL_SCOPE

    if (PyType_Ready(&GtpyStdOutRedirectType) < 0)
    {
        std::cerr << "could not initialize GtpyStdOutRedirectType" << ", in "
                  << __FILE__ << ":" << __LINE__ << std::endl;
    }

    Py_INCREF(&GtpyStdOutRedirectType);

    PythonQtObjectPtr sys;
    sys.setNewRef(PyImport_ImportModule("sys"));

    // create a redirection object for stdout and stderr
    m_out = GtpyStdOutRedirectType.tp_new(&GtpyStdOutRedirectType,NULL, NULL);
    ((GtpyStdOutRedirect*)m_out.object())->_cb = stdOutRedirectCB;

    m_err = GtpyStdOutRedirectType.tp_new(&GtpyStdOutRedirectType,NULL, NULL);
    ((GtpyStdOutRedirect*)m_err.object())->_cb = stdErrRedirectCB;

    // replace the built in file objects with the new objects
    PyModule_AddObject(sys, "stdout", m_out);
    PyModule_AddObject(sys, "stderr", m_err);
}

void
GtpyContextManager::importDefaultModules(
        const GtpyContextManager::Context& type)
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr con = context(type);

    if (con == Q_NULLPTR)
    {
        return;
    }

    QString pyCode =
        QStringLiteral("from PythonQt import ") +
            CLASS_WRAPPER_MODULE + QStringLiteral("\n") +
        QStringLiteral("from PythonQt import QtCore");

    con.evalScript(pyCode);
}

void
GtpyContextManager::importLoggingFuncs(const GtpyContextManager::Context& type,
                                        bool appConsole)
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr con = context(type);

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

    con.evalScript(pyCode);

    m_appLogging.insert(type, appConsole);
}

void
GtpyContextManager::importCalcModule(const GtpyContextManager::Context& type)
{
    GTPY_GIL_SCOPE

    if (!m_calcAccessibleContexts.contains(type))
    {
        m_calcAccessibleContexts << type;
    }

    PythonQtObjectPtr con = context(type);

    if (con == Q_NULLPTR)
    {
        return;
    }

    QString pyCode =
        "import sys\n"
        "if '" + CALC_MODULE + "' in sys.modules:\n"
        "    if sys.modules['" + CALC_MODULE + "'] is not None:\n"
        "        import " + CALC_MODULE + "\n"
        "        from " + CALC_MODULE + " import *\n"
        "        del " + CALC_FAC_VAR + "\n"
        "        del " + HELPER_FAC_VAR + "\n"
        "del sys\n";

    con.evalScript(pyCode);
}

void
GtpyContextManager::registerCalcModuleInSys(
        const GtpyContextManager::Context& type)
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr con = context(type);

    if (con == Q_NULLPTR)
    {
        return;
    }

    QString pyCode =
            "if '" + CALC_MODULE + "' in locals():\n" +
            "    import sys\n" +
            "    sys.modules['" + CALC_MODULE + "'] = " + CALC_MODULE + "\n" +
            "    del sys\n";

    con.evalScript(pyCode);
}

void
GtpyContextManager::removeCalcModuleFromSys()
{
    GTPY_GIL_SCOPE

    PythonQtObjectPtr con = context(GtpyContextManager::GlobalContext);

    if (con == Q_NULLPTR)
    {
        return;
    }

    QString pyCode =
        "import sys\n"
        "if '" + CALC_MODULE + "' in sys.modules:\n" +
        "    sys.modules['" + CALC_MODULE + "'] = None\n" +
        "del sys\n";

    con.evalScript(pyCode);
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
GtpyContextManager::setMetaDataToThreadDict(
        const GtpyContextManager::Context& type, bool output, bool error)
{
    GTPY_GIL_SCOPE

    PyObject* threadDict = PyThreadState_GetDict();

    QMap<QString, GtpyContextManager::Context> map = getContextNamesMap();

    QString contextName = map.key(type);

    PyObject* val = PyString_FromString(contextName.toStdString().c_str());

    PyDict_SetItemString(threadDict, GtpyStdOut::CONTEXT_KEY, val);

    val = PyBool_FromLong(output ? 1 : 0);

    PyDict_SetItemString(threadDict, GtpyStdOut::OUTPUT_KEY, val);

    val = PyBool_FromLong(error ? 1 : 0);

    PyDict_SetItemString(threadDict, GtpyStdOut::ERROR_KEY, val);

    Py_DECREF(val);
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

            if (isDict)
            {
                value = PyDict_GetItem(object, key);
                Py_INCREF(value);
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
                        QString type = QString::fromUtf8(info->
                                                     metaMethod()->typeName());
                        QString sig = info->fullSignature(true);

                        func.toolTip = type + QStringLiteral(" ") + sig;
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
                GtObject* obj = qvariant_cast<GtObject*>(var);

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
                                   GtObject::staticMetaObject.className()) +
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
                GtAbstractProperty* prop = qvariant_cast <
                                           GtAbstractProperty*> (var);

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
GtpyContextManager::builtInCompletions(
        const GtpyContextManager::Context& type) const
{
    GTPY_GIL_SCOPE

    QMultiMap<QString, GtpyFunction> results;

    // the script down below does not work in 2.7
    if (pythonVersion() != QStringLiteral("3.7"))
    {
        return results;
    }

    PythonQtObjectPtr context = this->context(type);

    if (context.isNull())
    {
        return results;
    }

    context.evalScript(QStringLiteral("import inspect"));
    context.evalScript(QStringLiteral("def __builtins():") +
                       QStringLiteral("return dir(inspect.builtins)"));

    QVariant v = context.call(QStringLiteral("__builtins"));

    context.evalScript(QStringLiteral("del inspect"));

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

        if(name.startsWith(c, Qt::CaseSensitive))
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
GtpyContextManager::setImportableModulesCompletions(
        const GtpyContextManager::Context& type)
{
    GTPY_GIL_SCOPE

    // the script down below does not work in 2.7
    if (pythonVersion() != QStringLiteral("3.7"))
    {
        return;
    }

    QMultiMap<QString, GtpyFunction> results;

    PythonQtObjectPtr con = context(type);

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
GtpyContextManager::calculatorCompletions(
        const GtpyContextManager::Context& type) const
{
    QMultiMap<QString, GtpyFunction> results;

    if (!m_calcAccessibleContexts.contains(type))
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
GtpyContextManager::setStandardCompletions(
        const GtpyContextManager::Context& type)
{
    QMultiMap<QString, GtpyFunction> results = builtInCompletions(type);
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

QString GtpyContextManager::pythonVersion() const
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
    if (!PythonQt::self())
    {
        std::cout << message.toLatin1().data() << std::endl;
        return;
    }

    if (output && !contextName.isEmpty())
    {
        QMap<QString, GtpyContextManager::Context> map = getContextNamesMap();

        GtpyContextManager::Context type = map.value(contextName);

        if (type == BatchContext)
        {
            if (message.indexOf(QStringLiteral("\n")) != 0 &&
                    !message.isEmpty())
            {
                std::cout << message.toLatin1().data() << std::endl;
            }

            return;
        }

        emit GtpyContextManager::instance()->pythonMessage(message, type);
    }
}

void
GtpyContextManager::stdErrRedirectCB(const QString& contextName,
                                     const bool /*output*/, const bool error,
                                     const QString& message)
{
    if (!PythonQt::self())
    {
        std::cerr << message.toLatin1().data() << std::endl;
        return;
    }

    if (error && !contextName.isEmpty())
    {
        QMap<QString, GtpyContextManager::Context> map = getContextNamesMap();

        GtpyContextManager::Context type = map.value(contextName);

        emit GtpyContextManager::instance()->errorMessage(message, type);

        int line = lineOutOfMessage(message);

        if (line > -1)
        {
            emit GtpyContextManager::instance()->errorCodeLine(line, type);
        }
    }
}

QMap<QString, GtpyContextManager::Context>
GtpyContextManager::initContextNamesMap()
{
    QMap<QString, GtpyContextManager::Context> contextNames;

    QMetaObject metaObj = GtpyContextManager::staticMetaObject;
    QMetaEnum metaEnum = metaObj.enumerator(
                             metaObj.indexOfEnumerator("Context"));

    int keyCount = metaEnum.keyCount();

    for (int i = 0;  i < keyCount; i++)
    {
        QString contextName = QString::fromUtf8(metaEnum.key(i));

        Context type = static_cast<Context>(metaEnum.value(i));

        contextNames.insert(contextName, type);
    }

    return contextNames;
}

QMap<QString, GtpyContextManager::Context>
GtpyContextManager::getContextNamesMap()
{
    static QMap<QString, GtpyContextManager::Context> CONTEXT_NAMES =
            initContextNamesMap();

    return CONTEXT_NAMES;
}

void
GtpyContextManager::onErrorMessage(const QString& message)
{
    GTPY_GIL_SCOPE

    PyObject* threadDict = PyThreadState_GetDict();

    PyObject* item = PyDict_GetItem(threadDict, PyString_FromString(
                                        GtpyStdOut::CONTEXT_KEY));

    GtpyContextManager::Context type;

    if (item)
    {
        const char* val = PyString_AsString(item);

        QMap<QString, GtpyContextManager::Context> map = getContextNamesMap();

        type = map.value(QString(val));
    }

    item = PyDict_GetItem(threadDict, PyString_FromString(
                              GtpyStdOut::ERROR_KEY));

    bool error = false;

    if (item && PyInt_Check(item))
    {
        error = (bool)PyInt_AsLong(item);
    }


    if (error)
    {
        emit errorMessage(message, type);
    }
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
