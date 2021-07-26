/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptcalculator.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gt_logging.h"
#include "gt_application.h"
#include "gt_package.h"
#include "gt_project.h"
#include "gt_objectpathproperty.h"
#include "gt_processdata.h"

#include "gtpy_contextmanager.h"
#include "gtpy_wizardgeometries.h"

#include "gtpy_scriptcalculator.h"

GtpyScriptCalculator::GtpyScriptCalculator() :
    m_script("script", "Script"),
    m_replaceTabBySpaces("replaceTab", "Replace tab by spaces"),
    m_tabSize("tabSize", "Tab size"),
    m_pyThreadId(-1)
{
    setObjectName("Python Script Editor");

    registerProperty(m_script);
    registerProperty(m_replaceTabBySpaces);
    registerProperty(m_tabSize);

    m_tabSize = 4;
    m_replaceTabBySpaces = true;

    m_script.hide();
    m_replaceTabBySpaces.hide();
    m_tabSize.hide();

    foreach (const QString& modId, getModuleIds())
    {
        QString modClass = gtApp->modulePackageId(modId);

        if (!modClass.isEmpty())
        {
            GtObjectPathProperty* pathProp =
                new GtObjectPathProperty(QStringLiteral(""),
                                         QStringLiteral(""),
                                         QStringLiteral(""),
                                         modId,
                                         this,
                                         QStringList() << modClass);
            pathProp->hide(true);
            registerProperty(*pathProp);

            m_dynamicPathProps << pathProp;
        }
    }

    connect(this, SIGNAL(stateChanged(GtProcessComponent::STATE)), this,
            SLOT(onStateChanged(GtProcessComponent::STATE)));

    connect(this, SIGNAL(deletedFromDatamodel(QString)),
            GtpyWizardGeometries::instance(),
            SLOT(processElementDeleted(QString)));
}

GtpyScriptCalculator::~GtpyScriptCalculator()
{
    qDeleteAll(m_dynamicPathProps);

    if (this->findParent<GtProcessData*>())
    {
        emit deletedFromDatamodel(this->uuid());
    }
}

bool
GtpyScriptCalculator::run()
{
    int contextId = GtpyContextManager::instance()->createNewContext(
                        GtpyContextManager::CalculatorRunContext, true);

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->addGtObject(contextId,
                    package->objectName(), package);
        }
    }

    gtInfo() << "running script...";

    m_pyThreadId = GtpyContextManager::instance()->currentPyThreadId();

    GtpyGlobals::StdOutMetaData metaData =
        GtpyContextManager::instance()->threadDictMetaData();

    bool success;

    success = GtpyContextManager::instance()->evalScript(
                  contextId, script(), true);

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->removeObject(contextId,
                    package->objectName());
        }
    }

    GtpyContextManager::instance()->deleteContext(contextId, true);

    GtpyContextManager::instance()->setMetaDataToThreadDict(metaData);

    gtInfo() << "...done!";

    return success ;
}

QString
GtpyScriptCalculator::script() const
{
    return m_script;
}

void
GtpyScriptCalculator::setScript(QString script)
{

#if GT_VERSION >= 0x010700

#else
    script.replace("\n", "\r");
#endif

    m_script = script;
}

QStringList
GtpyScriptCalculator::packageNames()
{
    QStringList list;

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        list.append(pathProp->path().toString());
    }

    return list;
}

bool
GtpyScriptCalculator::replaceTabBySpaces() const
{
    return m_replaceTabBySpaces;
}

void
GtpyScriptCalculator::setReplaceTabBySpaces(bool replaceTabBySpaces)
{
    m_replaceTabBySpaces = replaceTabBySpaces;
}

int
GtpyScriptCalculator::tabSize() const
{
    return m_tabSize;
}

void
GtpyScriptCalculator::setTabSize(int tabSize)
{
    m_tabSize = tabSize;
}


QStringList
GtpyScriptCalculator::getModuleIds()
{
    GtProject* project = gtApp->currentProject();

    if (project == Q_NULLPTR)
    {
        return QStringList();
    }

    return project->moduleIds();
}

void GtpyScriptCalculator::connectWithRootTask(bool connection)
{
    GtObject* root = findRoot<GtObject*>();

    if (!root)
    {
        return;
    }

    GtProcessComponent* rootTask = qobject_cast<GtProcessComponent*>(root);

    if (!rootTask)
    {
        return;
    }

    if (connection)
    {
        connect(rootTask, SIGNAL(stateChanged(GtProcessComponent::STATE)),
                this, SLOT(onTaskStateChanged(GtProcessComponent::STATE)));
    }
    else
    {
        disconnect(rootTask, SIGNAL(stateChanged(GtProcessComponent::STATE)),
                   this, SLOT(onTaskStateChanged(GtProcessComponent::STATE)));
    }
}

void
GtpyScriptCalculator::onStateChanged(GtProcessComponent::STATE state)
{
    bool connect = false;

    if (state == GtProcessComponent::RUNNING)
    {
        connect = true;
    }

    connectWithRootTask(connect);
}

void
GtpyScriptCalculator::onTaskStateChanged(GtProcessComponent::STATE state)
{
    if (m_pyThreadId < 0)
    {
        return;
    }

    if (state == GtProcessComponent::TERMINATION_REQUESTED)
    {
        GtpyContextManager::instance()->interruptPyThread(m_pyThreadId);
    }
}

