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

#include "gtpy_contextmanager.h"

#include "gtpy_scriptcalculator.h"

GtpyScriptCalculator::GtpyScriptCalculator() :
    m_script("script", "Script"),
    m_pyThreadId(-1)
{
    setObjectName("Python Script Editor");

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

    registerProperty(m_script);
    m_script.hide();

    connect(this, SIGNAL(stateChanged(GtProcessComponent::STATE)), this,
            SLOT(onStateChanged(GtProcessComponent::STATE)));
}

GtpyScriptCalculator::~GtpyScriptCalculator()
{
    qDeleteAll(m_dynamicPathProps);
}

bool
GtpyScriptCalculator::run()
{
    GtpyContextManager::Context type = GtpyContextManager::CalculatorRunContext;

    GtpyContextManager::instance()->resetContext(type);

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->addObject(type,
                        package->objectName(), package);
        }
    }

    gtInfo() << "running script...";

    m_pyThreadId = GtpyContextManager::instance()->currentPyThreadId();

    bool success;

    success = GtpyContextManager::instance()->evalScript(
                  type, script(), true);

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->removeObject(type,
                                             package->objectName());
        }
    }

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
    script.replace("\n", "\r");

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

