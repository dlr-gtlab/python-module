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
    m_script("script", "Script")
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
}

GtpyScriptCalculator::~GtpyScriptCalculator()
{
    qDeleteAll(m_dynamicPathProps);
}

bool
GtpyScriptCalculator::run()
{
    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->addObject(
                        GtpyContextManager::CalculatorRunContext,
                        package->objectName(), package);
        }
    }

    gtInfo() << "running script...";


    bool success;

    success = GtpyContextManager::instance()->evalScript
              (GtpyContextManager::CalculatorRunContext,
               script(), true);

    foreach (GtObjectPathProperty* pathProp, m_dynamicPathProps)
    {
        GtPackage* package = data<GtPackage*>(pathProp->path());

        if (package != Q_NULLPTR)
        {
            GtpyContextManager::instance()->removeObject(
                        GtpyContextManager::CalculatorRunContext,
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
GtpyScriptCalculator::setScript(const QString& script)
{
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

