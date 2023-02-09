/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 11.08.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <QApplication>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDir>

#include "gt_taskdata.h"
#include "gt_calculatordata.h"
#include "gt_application.h"
#include "gt_logging.h"
#if GT_VERSION >= 0x020000
#include "gt_commandlinefunction.h"
#endif

// data model classes

// calculator classes
#include "gtpy_scriptcalculator.h"

// task classes
#include "gtpy_task.h"

// ui classes
#include "gtpy_console.h"

// mdi items
#include "gt_extendedcalculatordata.h"
#include "gt_extendedtaskdata.h"
#include "gt_customprocesswizard.h"

// wizards
#include "gtpy_scriptcalculatorwizardpage.h"
#include "python_task/gtpy_taskwizardpage.h"

// post
#include "gtpy_pythonplotitem.h"
#include "gtpy_pythonplotdata.h"

// network
#include "gt_accessdataconnection.h"

#include "gtpy_scriptcollectionsettings.h"

#include "gt_python.h"

#if GT_VERSION >= 0x010700
GtVersionNumber
GtPythonModule::version()
{
    return GtVersionNumber(1, 3, 0);
}
#else
int
GtPythonModule::version()
{
    return 1;
}
#endif

QString
GtPythonModule::description() const
{
    return QStringLiteral("GTlab Python Module");
}

#if GT_VERSION >= 0x020000
GtPythonModule::MetaInformation
GtPythonModule::metaInformation() const
{
    MetaInformation m;
    m.author = "AT-TWK";
    return m;
}
#endif

void
GtPythonModule::init()
{
    gtDebug() << "python module initialization...";

    GtpyContextManager::instance()->initContexts();

#if GT_VERSION >= 0x020000
    if (gtApp->batchMode())
    {
        return;
    }
#endif

    QStringList widPath;
    widPath << "GtMainWin" << "Output" << "centralWidget" << "tabWidget";

    QWidget* outDock = findWidget(widPath);
//    gtDebug() << "FOUND WIDGET: " << outDock;

    QTabWidget* tab = qobject_cast<QTabWidget*>(outDock);

    if (!tab)
    {
        gtWarning() << "Output dock tab widget not found!";
        return;
    }

    gtDebug() << "FOUND WIDGET: " << tab->metaObject()->className() <<
        "(" << &tab << ", " << tab->objectName() << ")";

    // python console
    QWidget* pythonConsoleTab = new QWidget(tab);
    tab->addTab(pythonConsoleTab, "Python Console");
    QVBoxLayout* pythonConsoleLayout = new QVBoxLayout;
    pythonConsoleLayout->setContentsMargins(0, 0, 0, 0);
    pythonConsoleLayout->setSpacing(0);
    pythonConsoleTab->setLayout(pythonConsoleLayout);

    GtpyConsole* pythonConsole =
        new GtpyConsole(GtpyContextManager::GlobalContext,
                        pythonConsoleTab);

    pythonConsole->showAdditionalContextOutput(
        GtpyContextManager::CalculatorRunContext);
    pythonConsole->showAdditionalContextOutput(
        GtpyContextManager::TaskRunContext);

    connect(GtpyContextManager::instance(), SIGNAL(newContextCreated(int)),
            pythonConsole, SLOT(showAdditionalContextOutput(int)));
    connect(GtpyContextManager::instance(), SIGNAL(contextDeleted(int)),
            pythonConsole, SLOT(removeAdditionalContextOutput(int)));

    pythonConsoleLayout->addWidget(pythonConsole);

    // python console tools layout
    QHBoxLayout* pyToolLayout = new QHBoxLayout;
    pyToolLayout->setContentsMargins(0, 0, 0, 0);
    pyToolLayout->setSpacing(0);
    pyToolLayout->addStretch(1);

    // clear python console button
    QPushButton* pyClearButton = new QPushButton;
    pyClearButton->setIcon(gtApp->icon("clearIcon_16.png"));
    pyClearButton->setMaximumSize(QSize(20, 20));
    pyClearButton->setFlat(true);
    pyClearButton->setToolTip(tr("Clear Output"));
    pyToolLayout->addWidget(pyClearButton);

    pythonConsoleLayout->addLayout(pyToolLayout);

    connect(pyClearButton, SIGNAL(clicked(bool)), pythonConsole,
            SLOT(clearConsole()));
}

QList<GtCalculatorData>
GtPythonModule::calculators()
{
    QList<GtCalculatorData> metaData;

    // Python Script Runner
    GtExtendedCalculatorData python = GT_EXTENDED_CALC_DATA(
                                          GtpyScriptCalculator)
    python->id = QStringLiteral("Python Script Calculator");

#if GT_VERSION >= 0x010700
    python->version = GtVersionNumber(0, 1);
#else
    python->version = 0.1;
#endif

    python->author = QStringLiteral("Marvin Nöthen");
    python->icon = gtApp->icon(QStringLiteral("pythonIcon_16.png"));
    python->status = GtAbstractProcessData::RELEASE;

    GtCustomProcessWizard* pythonWizard = new GtCustomProcessWizard;
    pythonWizard->setPage(1, GtpyScriptCalculatorWizardPage::staticMetaObject);
    pythonWizard->setSize(1000, 750);
    python->wizard = pythonWizard;

    metaData << python;

    return metaData;
}

QList<GtTaskData>
GtPythonModule::tasks()
{
    QList<GtTaskData> metaData;

    GtExtendedTaskData pythonTask = GT_EXTENDED_TASK_DATA(GtpyTask)
    pythonTask->id = QStringLiteral("Python Task");

#if GT_VERSION >= 0x020000
    pythonTask->version = GtVersionNumber(0, 1);
#else
    pythonTask->version = 0.1;
#endif

    pythonTask->author = QStringLiteral("Marvin Nöthen");
    pythonTask->icon = gtApp->icon(QStringLiteral("pythonIcon_16.png"));
    pythonTask->status = GtAbstractProcessData::RELEASE;

    GtCustomProcessWizard* pythonWizard = new GtCustomProcessWizard;
    pythonWizard->setPage(1, GtpyTaskWizardPage::staticMetaObject);
    pythonWizard->setSize(1000, 750);
    pythonTask->wizard = pythonWizard;

    metaData << pythonTask;

    return metaData;
}

QList<QMetaObject>
GtPythonModule::mdiItems()
{
    QList<QMetaObject> metaData;

    return metaData;
}

QList<QMetaObject>
GtPythonModule::dockWidgets()
{
    QList<QMetaObject> metaData;

    return metaData;
}

QMap<const char*, QMetaObject>
GtPythonModule::uiItems()
{
    return {};
}

QList<QMetaObject>
GtPythonModule::postItems()
{
    QList<QMetaObject> retval;

    retval.append(GT_METADATA(GtpyPythonPlotData));

    return retval;
}

QList<QMetaObject>
GtPythonModule::postPlots()
{
    QList<QMetaObject> retval;

    retval.append(GT_METADATA(GtpyPythonPlotItem));

    return retval;
}

QIcon
GtPythonModule::collectionIcon() const
{
    return gtApp->icon(QStringLiteral("pythonIcon_16.png"));
}

QString
GtPythonModule::collectionId() const
{
    return GtpyGlobals::COLLECTION_ID;
}

QMetaObject
GtPythonModule::collectionSettings() const
{
    return GtpyScriptCollectionSettings::staticMetaObject;
}

QMap<QString, QMetaType::Type>
GtPythonModule::collectionStructure() const
{
    QMap<QString, QMetaType::Type> retval;

    retval.insert(GtpyGlobals::COLLECTION_cat, QMetaType::QString);
    retval.insert(GtpyGlobals::COLLECTION_subcat, QMetaType::QString);

    return retval;
}

QString
GtPythonModule::accessId()
{
    return collectionId();
}

QMetaObject
GtPythonModule::accessConnection()
{
    return GT_METADATA(GtAccessDataConnection);
}

namespace PythonExecution
{

QString
parseScriptFile(QFile& file)
{
    if (!file.exists())
    {
        gtError() << "ERROR: script file not found!";
        return QString();
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        gtError() << "ERROR: could not open script file!";
        return QString();
    }

    QTextStream out(&file);

    return out.readAll();
}

int
execPython(const QStringList& args)
{
    if (args.isEmpty())
    {
        return -1;
    }

    QFile f(args.first());

    gtInfo() << "Start Python Script Execution for file" << args.first();
    GtpyContextManager* python = GtpyContextManager::instance();

    python->initContexts();

    QString scriptContent = parseScriptFile(f);

    if (scriptContent.isEmpty())
    {
        gtError() << "ERROR: empty script file!";
        return -1;
    }

    if (python)
    {
        bool success = python->evalScript(GtpyContextManager::BatchContext,
                                          scriptContent, true);

        if (success)
        {
            return 0;
        }
    }

    return -1;
}
}

#if GT_VERSION >= 0x020000
QList<GtCommandLineFunction>
GtPythonModule::commandLineFunctions() const
{
    QList<GtCommandLineArgument> args;
    args.append(GtCommandLineArgument{"<file>", "python file to execute"});
    auto fun = gt::makeCommandLineFunction(
                "python", PythonExecution::execPython,
                "Executes python").setArgs(args);

    return {fun};
}
#endif

QWidget*
GtPythonModule::findWidget(QStringList path, QWidget* parent)
{
    if (path.isEmpty())
    {
        return parent;
    }

    if (!parent)
    {
        // no parent given... get all top level widgets from application
        QWidgetList wids = QApplication::topLevelWidgets();
        QString firstId = path.takeFirst();

        foreach (QWidget* wid, wids)
        {
            if (wid->objectName() == firstId)
            {
                return findWidget(path, wid);
            }
        }
    }

    QString widId = path.takeFirst();
    QWidget* child = parent->findChild<QWidget*>(widId,
                     Qt::FindDirectChildrenOnly);

    if (!child)
    {
        gtWarning() << "Could not find target widget!";
        return nullptr;
    }

    return findWidget(path, child);
}

