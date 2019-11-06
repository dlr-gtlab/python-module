/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_decorator.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QDebug>
#include <QMetaMethod>
#include <QStringList>

#include "gt_coreapplication.h"
#include "gt_coredatamodel.h"
#include "gt_project.h"
#include "gt_task.h"
#include "gt_coreprocessexecutor.h"
#include "gt_processdata.h"
#include "gt_calculator.h"
#include "gt_objectlinkproperty.h"
#include "gt_command.h"
#include "gt_datazone0d.h"

#include "gtpy_decorator.h"

GtpyDecorator::GtpyDecorator(QObject* parent) : QObject(parent)
{

}

QString
GtpyDecorator::getFunctionName(const QString& funcTag)
{
    int methodCount = metaObject()->methodCount();

    for (int i= 0; i < methodCount; i++)
    {
        const QMetaObject* metaObj = metaObject();
        QMetaMethod metaMeth = metaObj->method(i);
        const char* tag = metaMeth.tag();
        QString myString = QString::fromUtf8(tag);

        if (myString == funcTag)
        {
            return QString::fromUtf8(metaMeth.name());
        }
    }

    return QString();
}

void
GtpyDecorator::init(GtCoreApplication* app, const QString& id)
{
    if (app == Q_NULLPTR)
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("app is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return;
    }

    qDebug() << "Decorator:     app->init()";
    app->init();

    initLanguage(app);
    initDatamodel(app);
    loadModules(app);
    initCalculators(app);
    initSession(app, id);
}

void
GtpyDecorator::initLanguage(GtCoreApplication* app)
{
    if (app == Q_NULLPTR)
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("app is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return;
    }

    qDebug() << "Decorator:     app->initLanguage()";
    app->initLanguage();
}

void
GtpyDecorator::initDatamodel(GtCoreApplication* app)
{
    if (app == Q_NULLPTR)
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("app is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return;
    }

    qDebug() << "Decorator:     app->initDatamodel()";
    app->initDatamodel();
}

void
GtpyDecorator::loadModules(GtCoreApplication* app)
{
    if (app == Q_NULLPTR)
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("app is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return;
    }

    qDebug() << "Decorator:     app->loadModules()";
    app->loadModules();
}

void
GtpyDecorator::initCalculators(GtCoreApplication* app)
{
    if (app == Q_NULLPTR)
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("app is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return;
    }

    qDebug() << "Decorator:     app->initCalculators()";
    app->initCalculators();
}

void
GtpyDecorator::initSession(GtCoreApplication* app, const QString& id)
{
    if (app == Q_NULLPTR)
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("app is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return;
    }

    qDebug() << "Decorator:     app->initSession(id) : id == " << id;
    app->initSession(id);
}

void
GtpyDecorator::switchSession(GtCoreApplication* app, const QString& id)
{
    if (app == Q_NULLPTR)
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("app is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return;
    }

    qDebug() << "Decorator:     app->switchSession(id) : id == " << id;
    app->switchSession(id);
}

GtProject*
GtpyDecorator::openProject(GtCoreApplication* app,
                                const QString& projectId)
{
    if (app == Q_NULLPTR)
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("helper is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return Q_NULLPTR;
    }

    qDebug() << "open project...     projectId == " << projectId;

    if (projectId.isEmpty())
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("project id is empty!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return Q_NULLPTR;
    }

    GtProject* project = app->findProject(projectId);

    if (project == Q_NULLPTR)
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("project not found!") +
                   QStringLiteral(" (") + projectId + QStringLiteral(")");

        qWarning() << output;

        emit sendErrorMessage(output);

        return Q_NULLPTR;
    }

    if (!gtDataModel->GtCoreDatamodel::openProject(project))
    {
        QString output = QStringLiteral("ERROR: ") +
                         QObject::tr("could not open project!") +
                         QStringLiteral(" (") + projectId +
                         QStringLiteral(")");

        qWarning() << output;

        emit sendErrorMessage(output);

        return Q_NULLPTR;
    }

    qDebug() << "project opened!";

    return project;
}

GtProject*
GtpyDecorator::currentProject(GtCoreApplication* app)
{
    if (app == Q_NULLPTR)
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("app is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return Q_NULLPTR;
    }

    return app->currentProject();
}

const QString
GtpyDecorator::path(GtProject* pro)
{
    if (pro == Q_NULLPTR)
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("pro is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return QString();
    }

    return pro->path();
}

bool
GtpyDecorator::isValid(GtProject* pro)
{
    if (pro == Q_NULLPTR)
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("pro is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return false;
    }

    return pro->isValid();
}

bool
GtpyDecorator::isOpen(GtProject* pro)
{
    if (pro == Q_NULLPTR)
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("pro is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return false;
    }

    return pro->isOpen();
}

bool
GtpyDecorator::runProcess(GtProject* pro, const QString& processId,
                               bool save)
{
    if (pro == Q_NULLPTR)
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("pro is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return false;
    }

    if (processId.isEmpty())
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("processId is empty!");

        qWarning() << output;

        emit sendErrorMessage(output);

    }

    qDebug() << "process run starts...      processId == " << processId;
    // run process

    GtTask* process = pro->findProcess(processId);

    if (process == Q_NULLPTR)
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("process not found!") +
                          QStringLiteral(" (") + processId +
                          QStringLiteral(")");

        qWarning() << output;

        emit sendErrorMessage(output);

        return false;
    }

    // execute process
    GtCoreProcessExecutor executor;
    executor.runTask(process);

    if (process->currentState() != GtProcessComponent::FINISHED)
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("process run failed!") +
                          QStringLiteral(" (") + processId +
                          QStringLiteral(")");

        qWarning() << output;

        emit sendErrorMessage(output);

        return false;
    }

    qDebug() << "process run successful!";

    if (save)
    {
        if (!gtDataModel->saveProject(pro))
        {
            QString output = QStringLiteral("ERROR: ") +
                       QObject::tr("project could not besaved!") +
                       QStringLiteral(" (") + pro->objectName() +
                       QStringLiteral(")");

            qWarning() << output;

            emit sendErrorMessage(output);

            return false;
        }
    }

    return true;
}

GtTask*
GtpyDecorator::findProcess(GtProject* pro, const QString& processId)
{
    if (pro == Q_NULLPTR || processId.isEmpty())
    {
        return Q_NULLPTR;
    }

    return pro->findProcess(processId);
}

bool
GtpyDecorator::run(GtCalculator* calc)
{
    if (calc == Q_NULLPTR)
    {
        return false;
    }

    return calc->exec();
}

//bool
//GtpyDecorator::runProcess(GtTask* process)
//{
//    if (process == Q_NULLPTR)
//    {
//        QString output =  QStringLiteral("ERROR: ") +
//                          QObject::tr("process not found!");

//        qWarning() << output;

//        emit sendErrorMessage(output);

//        return false;
//    }

//    // execute process
//    GtCoreProcessExecutor executor;
//    executor.runTask(process);

//    if (process->currentState() != GtProcessComponent::FINISHED)
//    {
//        QString output =  QStringLiteral("ERROR: ") +
//                          QObject::tr("process run failed!") +
//                          QStringLiteral(" (") + process->objectName() +
//                          QStringLiteral(")");

//        qWarning() << output;

//        emit sendErrorMessage(output);

//        return false;
//    }

//    qDebug() << "process run successful!";

//    return true;
//}

bool
GtpyDecorator::close(GtProject* pro, bool save)
{
    if (pro == Q_NULLPTR)
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("pro is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return false;
    }

    if (save)
    {
        if (!gtDataModel->saveProject(pro))
        {
            QString output = QStringLiteral("ERROR: ") +
                       QObject::tr("project could not besaved!") +
                       QStringLiteral(" (") + pro->objectName() +
                       QStringLiteral(")");

            qWarning() << output;

            emit sendErrorMessage(output);

            return false;
        }
    }

    if (!gtDataModel->closeProject(pro))
    {
        QString output = QStringLiteral("ERROR: ") +
                   QObject::tr("could not close project!") +
                   QStringLiteral(" (") + pro->objectName() +
                   QStringLiteral(")");

        qWarning() << output;

        emit sendErrorMessage(output);

        return false;
    }

    return true;
}

GtObject*
GtpyDecorator::findGtChild(GtObject* obj, const QString& childName)
{
    if (obj == Q_NULLPTR || childName.isEmpty())
    {
        return Q_NULLPTR;
    }

    return obj->findDirectChild<GtObject*>(childName);
}

QList<GtObject*>
GtpyDecorator::findGtChildren(GtObject* obj, const QString& childrenName)
{
    if (obj == Q_NULLPTR)
    {
        return QList<GtObject*>();
    }

    return obj->findDirectChildren<GtObject*>(childrenName);
}

QList<GtAbstractProperty*>
GtpyDecorator::findGtProperties(GtObject* obj)
{
    if (obj == Q_NULLPTR)
    {
        return QList<GtAbstractProperty*>();
    }

    return obj->fullPropertyList();
}

GtAbstractProperty*
GtpyDecorator::findGtProperty(GtObject* obj, const QString& id)
{
    if (obj == Q_NULLPTR || id.isEmpty())
    {
        return Q_NULLPTR;
    }

    return obj->findProperty(id);
}

QVariant
GtpyDecorator::propertyValue(GtObject* obj, const QString& id)
{
    if (obj == Q_NULLPTR || id.isEmpty())
    {
        return QVariant();
    }

    GtAbstractProperty* prop = obj->findProperty(id);

    if (prop == Q_NULLPTR)
    {
        return QVariant();
    }

    return prop->valueToVariant();
}

void
GtpyDecorator::setPropertyValue(GtObject* obj, const QString& id,
                                     QVariant val)
{
    if (obj == Q_NULLPTR || id.isEmpty())
    {
        return;
    }

    GtAbstractProperty* prop = obj->findProperty(id);

    if (prop == Q_NULLPTR)
    {
        return;
    }

    if (GtObjectLinkProperty* objLinkProp =
            qobject_cast<GtObjectLinkProperty*>(prop))
    {
        GtObject* dataObj = qvariant_cast<GtObject*>(val);

        if (dataObj)
        {
            QStringList allowedClasses = objLinkProp->allowedClasses();

            QString objClassName = QString::fromUtf8(
                                       dataObj->metaObject()->className());

            if (allowedClasses.contains(objClassName))
            {
                val = dataObj->uuid();
            }
            else
            {
                QString output =  QStringLiteral("ERROR: ") +
                                  objClassName +
                                  QObject::tr(" is not an allowed class "
                                              "type for the property ") +
                                  prop->objectName() + QObject::tr("!") +
                                  QObject::tr(" Allowed class types: ");

                QString temp;

                for (int i = 0; i < allowedClasses.size(); i++)
                {
                    if (i != 0)
                    {
                        temp += QStringLiteral(", ");
                    }

                    temp += allowedClasses.at(i);
                }

                output += temp;

                qWarning() << output;

                emit sendErrorMessage(output);

                return;
            }
        }
    }

    bool success = false;

//    GtCommand cmmd = gtApp->startCommand(
//                gtApp->currentProject(), prop->objectName() +
//                         QStringLiteral(" of ") + obj->objectName() +
//                         QStringLiteral(" changed!"));

    success = prop->setValueFromVariant(val, QString(), &success);

//    gtApp->endCommand(cmmd);

    if (!success)
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("Invalid input type for ") +
                          prop->objectName() + QObject::tr("!");

        emit sendErrorMessage(output);
    }
}

QString
GtpyDecorator::uuid(GtObject* obj)
{
    return obj->uuid();
}

QString
GtpyDecorator::calcHash(GtObject* obj)
{
    return obj->calcHash();
}

bool
GtpyDecorator::hasChanges(GtObject* obj)
{
    return obj->hasChanges();
}

void
GtpyDecorator::acceptChangesRecursively(GtObject* obj)
{
    return obj->acceptChangesRecursively();
}

QList<GtAbstractProperty*>
GtpyDecorator::findGtProperties(GtAbstractProperty* prop)
{
    if (prop == Q_NULLPTR)
    {
        return QList<GtAbstractProperty*>();
    }

    return prop->properties();
}

GtAbstractProperty*
GtpyDecorator::findGtProperty(GtAbstractProperty* prop, const QString& id)
{
    if (prop == Q_NULLPTR || id.isEmpty())
    {
        return Q_NULLPTR;
    }

    return prop->findProperty(id);
}

QVariant
GtpyDecorator::propertyValue(GtAbstractProperty* prop, const QString& id)
{
    if (prop == Q_NULLPTR || id.isEmpty())
    {
        return QVariant();
    }

    GtAbstractProperty* subProp = prop->findProperty(id);

    if (subProp == Q_NULLPTR)
    {
        return QVariant();
    }

    return subProp->valueToVariant();
}

void
GtpyDecorator::setPropertyValue(GtAbstractProperty* prop,
                                     const QString& id, QVariant val)
{
    if (prop == Q_NULLPTR || id.isEmpty())
    {
        return;
    }

    GtAbstractProperty* subProp = prop->findProperty(id);

    if (subProp == Q_NULLPTR)
    {
        return;
    }

    if (GtObjectLinkProperty* objLinkProp =
            qobject_cast<GtObjectLinkProperty*>(subProp))
    {
        GtObject* dataObj = qvariant_cast<GtObject*>(val);

        if (dataObj)
        {
            QStringList allowedClasses = objLinkProp->allowedClasses();

            QString objClassName = QString::fromUtf8(
                                       dataObj->metaObject()->className());

            if (allowedClasses.contains(objClassName))
            {
                val = dataObj->uuid();
            }
            else
            {
                QString output =  QStringLiteral("ERROR: ") +
                                  objClassName +
                                  QObject::tr(" is not an allowed class "
                                              "type for the property ") +
                                  subProp->objectName() + QObject::tr("!") +
                                  QObject::tr(" Allowed class types: ");

                QString temp;

                for (int i = 0; i < allowedClasses.size(); i++)
                {
                    if (i != 0)
                    {
                        temp += QStringLiteral(", ");
                    }

                    temp += allowedClasses.at(i);
                }

                output += temp;

                qWarning() << output;

                emit sendErrorMessage(output);

                return;
            }
        }
    }

    bool success = false;

//    GtCommand cmmd = gtApp->startCommand(
//                gtApp->currentProject(), prop->objectName() +
//                         QStringLiteral(" of ") + obj->objectName() +
//                         QStringLiteral(" changed!"));

    subProp->setValueFromVariant(val, QString(), &success);

//    gtApp->endCommand(cmmd);

    if (!success)
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("Invalid input type for ") +
                          subProp->objectName() + QObject::tr("!");

        emit sendErrorMessage(output);
    }
}

QString
GtpyDecorator::ident(GtAbstractProperty* prop)
{
    if (prop == Q_NULLPTR)
    {
        return QString();
    }

    return prop->ident();
}

QList<QObject*>
GtpyDecorator::getChildren(QObject* obj)
{
    return obj->findChildren<QObject*>(QString(), Qt::FindDirectChildrenOnly);
}

void
GtpyDecorator::setObjectName(QObject* obj, QString name)
{
    if (obj == Q_NULLPTR)
    {
        return;
    }

    obj->setObjectName(name);
}

double
GtpyDecorator::value(GtDataZone0D* dataZone, const QString& paramName,
                     bool* ok)
{
    if (dataZone == Q_NULLPTR)
    {
        if (ok)
        {
            *ok = false;
        }

        return 0.0;
    }

    return dataZone->value(paramName, ok);
}


void
GtpyDecorator::static_GtPythonLogger_gtPyDebug(const QVariant& mes)
{
    GtPythonLogger::gtPyDebug(mes);
}

void
GtpyDecorator::static_GtPythonLogger_gtPyInfo(const QVariant& mes)
{
    GtPythonLogger::gtPyInfo(mes);
}

void
GtpyDecorator::static_GtPythonLogger_gtPyError(const QVariant& mes)
{
    GtPythonLogger::gtPyError(mes);
}

void
GtpyDecorator::static_GtPythonLogger_gtPyFatal(const QVariant& mes)
{
    GtPythonLogger::gtPyFatal(mes);
}

void
GtpyDecorator::static_GtPythonLogger_gtPyWarning(const QVariant& mes)
{
    GtPythonLogger::gtPyWarning(mes);
}

void
GtPythonLogger::gtPyDebug(const QVariant& mes)
{
    gtDebug() << mes.toString();
}

void
GtPythonLogger::gtPyInfo(const QVariant& mes)
{
    gtInfo() << mes.toString();
}

void
GtPythonLogger::gtPyError(const QVariant& mes)
{
    gtError() << mes.toString();
}

void
GtPythonLogger::gtPyFatal(const QVariant& mes)
{
    gtFatal() << mes.toString();
}

void GtPythonLogger::gtPyWarning(const QVariant& mes)
{
    gtWarning() << mes.toString();
}
