/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_task.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gt_application.h"
#include "gt_objectpathproperty.h"
#include "gt_package.h"
#include "gt_project.h"
#include "gt_processdata.h"

#include "gtpy_contextmanager.h"
#include "gtpy_wizardgeometries.h"

#include "gtpy_task.h"

GtpyTask::GtpyTask():
    m_script("script", "Skript"),
    m_replaceTabBySpaces("replaceTab", "Replace tab by spaces"),
    m_tabSize("tabSize", "Tab size"),
    m_calcDefinitions("calcDefinitions", "Calculator Definitions"),
    m_pyThreadId(-1)
{
    setObjectName("Python Task");

    registerProperty(m_script);
    registerProperty(m_calcDefinitions);
    registerProperty(m_replaceTabBySpaces);
    registerProperty(m_tabSize);

    m_tabSize = 4;
    m_replaceTabBySpaces = true;

    m_script.hide();
    m_calcDefinitions.hide();
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

GtpyTask::~GtpyTask()
{
    qDeleteAll(m_dynamicPathProps);

    if (this->findParent<GtProcessData*>())
    {
        emit deletedFromDatamodel(this->uuid());
    }
}

bool
GtpyTask::runIteration()
{
    ///Create new context
    int contextId = GtpyContextManager::instance()->createNewContext(
                        GtpyContextManager::TaskRunContext, true);

    ///Initialize context with data model objects
    GtpyContextManager::instance()->addTaskValue(contextId, this);

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->addGtObject(contextId,
                    package->objectName(), package);
        }
    }

    ///Save the current thread settings
    m_pyThreadId = GtpyContextManager::instance()->currentPyThreadId();

    GtpyGlobals::StdOutMetaData metaData =
        GtpyContextManager::instance()->threadDictMetaData();

    ///Running Script
    gtInfo() << "running script...";

    bool success = true;

    success = GtpyContextManager::instance()->evalScript(
                  contextId, script(), true);

    ///Remove the data model objects from the context
    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->removeObject(contextId,
                    package->objectName());
        }
    }

    ///Delete the context
    GtpyContextManager::instance()->deleteContext(contextId, true);

    ///Reset the thread settings
    GtpyContextManager::instance()->setMetaDataToThreadDict(metaData);

    gtInfo() << "...done!";

    return success;
}

QString
GtpyTask::script() const
{
    return m_script;
}

void
GtpyTask::setScript(QString script)
{

#if GT_VERSION >= 0x010700

#else
    script.replace("\n", "\r");
#endif

    m_script = script;
}

QStringList
GtpyTask::packageNames()
{
    QStringList list;

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        list.append(pathProp->path().toString());
    }

    return list;
}

bool
GtpyTask::childAccepted(GtObject* /*child*/)
{
#if GT_VERSION >= 0x020000
    return qobject_cast<GtTaskGroup*>(this->parent()) == nullptr;
#else
    return qobject_cast<GtProcessData*>(this->parent()) == nullptr;
#endif
}

QStringList
GtpyTask::getModuleIds()
{
    GtProject* project = gtApp->currentProject();

    if (project == Q_NULLPTR)
    {
        return QStringList();
    }

    return project->moduleIds();
}

QString
GtpyTask::calcDefinitions() const
{
    return m_calcDefinitions;
}

void
GtpyTask::setCalcDefinitions(QString& calcDefinitions)
{

#if GT_VERSION >= 0x010700

#else
    calcDefinitions.replace("\n", "\r");
#endif

    m_calcDefinitions = calcDefinitions;
}

bool
GtpyTask::replaceTabBySpaces() const
{
    return m_replaceTabBySpaces;
}

void
GtpyTask::setReplaceTabBySpaces(bool replaceTabBySpaces)
{
    m_replaceTabBySpaces = replaceTabBySpaces;
}

int
GtpyTask::tabSize() const
{
    return m_tabSize;
}

void
GtpyTask::setTabSize(int tabSize)
{
    m_tabSize = tabSize;
}

void
GtpyTask::onStateChanged(STATE state)
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

