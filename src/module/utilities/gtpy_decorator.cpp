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

#include "PythonQtPythonInclude.h"

#include "gt_coreapplication.h"
#include "gt_coredatamodel.h"
#include "gt_project.h"
#include "gt_task.h"
#include "gt_coreprocessexecutor.h"
#include "gt_calculator.h"
#include "gt_objectlinkproperty.h"
#include "gt_abstractrunnable.h"
#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
#include "gt_datazone0d.h"
#endif

#include "gtpy_processdatadistributor.h"
#include "gtpy_extendedwrapper.h"
#include "gtpy_task.h"
#include "gtpy_scriptcalculator.h"
#include "gtpy_convert.h"

#include "gtpy_decorator.h"

using namespace GtpyExtendedWrapperModule;

GtpyDecorator::GtpyDecorator(QObject* parent) : QObject(parent)
{

}

QString
GtpyDecorator::getFunctionName(const QString& funcTag)
{
    int methodCount = metaObject()->methodCount();

    for (int i = 0; i < methodCount; i++)
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

GtObject*
GtpyDecorator::pyObjectToGtObject(PythonQtObjectPtr obj)
{
    if (obj.isNull())
    {
        return Q_NULLPTR;
    }

    GtObject* gtObj = Q_NULLPTR;

    if (obj->ob_type == &GtpyExtendedWrapper_Type)
    {
        GtpyExtendedWrapper* self = (GtpyExtendedWrapper*)obj.object();

        if (self && self->_obj)
        {
            gtObj = qobject_cast<GtObject*>(self->_obj->_obj);
        }
    }

    return gtObj;
}

PyObject*
GtpyDecorator::wrapGtObject(GtObject* obj)
{
    if (!obj)
    {
        return Q_NULLPTR;
    }

    PyObject* pyQtWrapper = PythonQt::priv()->wrapQObject(obj);

    if (pyQtWrapper && pyQtWrapper->ob_type->tp_base !=
            &PythonQtInstanceWrapper_Type)
    {
        Py_XDECREF(pyQtWrapper);
        return Q_NULLPTR;
    }

    PyObject* childArg = PyTuple_New(1);

    PyTuple_SetItem(childArg, 0, pyQtWrapper);

    PyObject* wrapped = GtpyExtendedWrapper_Type.tp_new(
                            &GtpyExtendedWrapper_Type, childArg, Q_NULLPTR);

    Py_DECREF(childArg);

    return wrapped;
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

PyObject* GtpyDecorator::openProject(GtCoreApplication* app,
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

    return wrapGtObject(project);
}

PyObject*
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

    return wrapGtObject(app->currentProject());
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

PyObject*
GtpyDecorator::findProcess(GtProject* pro, const QString& processId)
{
    if (pro == Q_NULLPTR || processId.isEmpty())
    {
        return Q_NULLPTR;
    }

    return wrapGtObject(pro->findProcess(processId));
}

bool
GtpyDecorator::run(GtCalculator* calc)
{
    bool success = false;

    if (calc != Q_NULLPTR)
    {
        Py_BEGIN_ALLOW_THREADS

        if (calc->findParent<GtAbstractRunnable*>())
        {
            success = calc->exec();
        }

        Py_END_ALLOW_THREADS
    }

    return success;
}

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
PyObject*
GtpyDecorator::inputArgs(GtpyScriptCalculator* calc) const
{
    return gtpy::convert::fromPropertyStructContainer(calc->inputArgs());
}

QVariant
GtpyDecorator::inputArg(GtpyScriptCalculator* calc, const QString& argName) const
{
    QVariant value = calc->inputArg(argName);

    if (!value.isValid())
    {
        throw std::runtime_error(
                    tr("\n\nKeyError: '%1'\n"
                       "%2 has no input argument with the name '%1'")
                    .arg(argName, calc->objectName())
                    .toStdString());
    }

    return value;
}

void
GtpyDecorator::setInputArg(GtpyScriptCalculator* calc, const QString& argName,
                           const QVariant& value)
{
    if (!calc->setInputArg(argName, value))
    {
        throw std::runtime_error(
                    tr("\n\nKeyError: '%1'\n"
                       "%2 has no input argument with the name '%1'")
                    .arg(argName, calc->objectName())
                    .toStdString());
    }
}

PyObject*
GtpyDecorator::outputArgs(GtpyScriptCalculator* calc) const
{
    return gtpy::convert::fromPropertyStructContainer(calc->outputArgs());
}

QVariant
GtpyDecorator::outputArg(GtpyScriptCalculator* calc, const QString& argName) const
{
    QVariant value = calc->outputArg(argName);

    if (!value.isValid())
    {
        throw std::runtime_error(
                    tr("\n\nKeyError: '%1'\n"
                       "%2 has no output argument with the name '%1'")
                    .arg(argName, calc->objectName())
                    .toStdString());
    }

    return value;
}
#endif

bool
GtpyDecorator::run(GtTask* task)
{
    bool success = false;

    if (task != Q_NULLPTR)
    {
        Py_BEGIN_ALLOW_THREADS

        if (task->findParent<GtAbstractRunnable*>())
        {
            success = task->exec();
        }

        Py_END_ALLOW_THREADS
    }

    return success;
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

void
GtpyDecorator::deleteAllCalculators(GtTask* task)
{
    if (task == Q_NULLPTR)
    {
        return;
    }

    Py_BEGIN_ALLOW_THREADS

    QList<GtProcessComponent*> calcs = task->findDirectChildren <
                                       GtProcessComponent* > ();

    int lastIndex = calcs.size() - 1;

    for (lastIndex; lastIndex >= 0; lastIndex--)
    {
        GtProcessComponent* calc = calcs.at(lastIndex);
        calcs.removeAt(lastIndex);
        delete calc;
    }

    Py_END_ALLOW_THREADS
}

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
PyObject*
GtpyDecorator::inputArgs(GtpyTask* task) const
{
    return gtpy::convert::fromPropertyStructContainer(task->inputArgs());
}

QVariant
GtpyDecorator::inputArg(GtpyTask* task, const QString& argName) const
{
    QVariant value = task->inputArg(argName);

    if (!value.isValid())
    {
        throw std::runtime_error(
                    tr("\n\nKeyError: '%1'\n"
                       "%2 has no input argument with the name '%1'")
                    .arg(argName, task->objectName())
                    .toStdString());
    }

    return value;
}

void
GtpyDecorator::setInputArg(GtpyTask* task, const QString& argName,
                           const QVariant& value)
{
    if (!task->setInputArg(argName, value))
    {
        throw std::runtime_error(
                    tr("\n\nKeyError: '%1'\n"
                       "%2 has no input argument with the name '%1'")
                    .arg(argName, task->objectName())
                    .toStdString());
    }
}

PyObject*
GtpyDecorator::outputArgs(GtpyTask* task) const
{
    return gtpy::convert::fromPropertyStructContainer(task->outputArgs());
}

QVariant
GtpyDecorator::outputArg(GtpyTask* task, const QString& argName) const
{
    QVariant value = task->outputArg(argName);

    if (!value.isValid())
    {
        throw std::runtime_error(
                    tr("\n\nKeyError: '%1'\n"
                       "%2 has no output argument with the name '%1'")
                    .arg(argName, task->objectName())
                    .toStdString());
    }

    return value;
}
#endif


bool
GtpyDecorator::hasWarnings(GtProcessComponent* comp)
{
    if (!comp)
    {
        return false;
    }

    if (comp->currentState() == GtProcessComponent::WARN_FINISHED ||
            comp->currentState() == GtProcessComponent::FAILED)
    {
        return true;
    }

    foreach (GtProcessComponent* child,
             comp->findChildren<GtProcessComponent*>())
    {
        if (child->currentState() == GtProcessComponent::WARN_FINISHED ||
                child->currentState() == GtProcessComponent::FAILED)
        {
            return true;
        }
    }

    return false;
}


PythonQtPassOwnershipToPython<GtpyProcessDataDistributor*>
GtpyDecorator::new_GtpyProcessDataDistributor(GtpyTask* pythonTask)
{
    return new GtpyProcessDataDistributor(qobject_cast<GtTask*>(pythonTask));
}

void
GtpyDecorator::delete_GtpyProcessDataDistributor(
    GtpyProcessDataDistributor* obj)
{
    delete obj;
}

PyObject*
GtpyDecorator::taskElement(
    GtpyProcessDataDistributor* obj, const QString& name)
{
    GtTask* task = obj->taskElement(name);

    return wrapGtObject(task);
}

PyObject*
GtpyDecorator::findGtChild(GtObject* obj, const QString& childName)
{
    if (obj == Q_NULLPTR || childName.isEmpty())
    {
        return Q_NULLPTR;
    }

    GtObject* child = obj->findDirectChild<GtObject*>(childName);
    return wrapGtObject(child);
}

QList<PyObject*>
GtpyDecorator::findGtChildren(GtObject* obj, const QString& childrenName,
                              const QString& objectClassName)
{
    if (obj == Q_NULLPTR)
    {
        return QList<PyObject*>();
    }

    QList<GtObject*> children;

    if (childrenName.isEmpty())
    {
        children = obj->findDirectChildren<GtObject*>();
    }
    else
    {
        children = obj->findDirectChildren<GtObject*>(childrenName);
    }


    if (!objectClassName.trimmed().isEmpty())
    {
        for (auto it = children.begin(); it < children.end();)
        {
            GtObject* loopObj = (*it);

            if (!loopObj)
            {
                it = children.erase(it);

                continue;
            }

            if (QString(loopObj->metaObject()->className()).trimmed() !=
                    objectClassName.trimmed())
            {
                it = children.erase(it);

                continue;
            }

            ++ it;
        }
    }

    QList<PyObject*> retval;

    foreach (GtObject* child, children)
    {
        retval.append(wrapGtObject(child));
    }

    return retval;
}

PyObject*
GtpyDecorator::findGtParent(GtObject* obj)
{
    if (!obj)
    {
        gtError() << __func__ << " -> Invalid object given!";

        return nullptr;
    }

    return wrapGtObject(obj->findParent<GtObject*>());
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
        QString message = "\n\n" + QString(obj->metaObject()->className()) +
                          " has no GtProperty with the id: " + id;

        throw std::runtime_error(message.toStdString());

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
        QString message = "\n\n" + QString(obj->metaObject()->className()) +
                          " has no GtProperty with the id: " + id +
                          "\nIf you want to set a QProperty, use the function "
                          "setProperty()!";

        throw std::runtime_error(message.toStdString());

        return;
    }

    GtObjectLinkProperty* objLinkProp =
                    qobject_cast<GtObjectLinkProperty*>(prop);

    if (objLinkProp)
    {
        GtObject* dataObj = qvariant_cast<GtObject*>(val);

        if (!dataObj)
        {
            if (PythonQtObjectPtr().fromVariant(val))
            {
                dataObj = pyObjectToGtObject(val);
            }
        }

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

    //    GtCommand cmmd = gtApp->startCommand(
    //                gtApp->currentProject(), prop->objectName() +
    //                         QStringLiteral(" of ") + obj->objectName() +
    //                         QStringLiteral(" changed!"));

    bool success = prop->setValueFromVariant(val, QString());

    //    gtApp->endCommand(cmmd);

    if (!success)
    {
        QString output = QStringLiteral("ERROR: ") +
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

PyObject*
GtpyDecorator::objectByUUID(GtObject* obj, const QString& uuid)
{
    Q_UNUSED(obj)

    return wrapGtObject(obj->getObjectByUuid(uuid));
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

QString
GtpyDecorator::className(GtObject* obj)
{
    if (obj == Q_NULLPTR)
    {
        return QString();
    }

    return obj->metaObject()->className();
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
        QString message = "\n\n" + QString(prop->metaObject()->className()) +
                          " has no subproperty of type GtProperty with the "
                          "id: " + id;

        throw std::runtime_error(message.toStdString());

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
        QString message = "\n\n" + QString(prop->metaObject()->className()) +
                          " has no subproperty of type GtProperty with the "
                          "id: " + id;

        throw std::runtime_error(message.toStdString());

        return;
    }

    GtObjectLinkProperty* objLinkProp =
                    qobject_cast<GtObjectLinkProperty*>(subProp);

    if (objLinkProp)
    {
        GtObject* dataObj = qvariant_cast<GtObject*>(val);

        if (!dataObj)
        {
            if (PythonQtObjectPtr().fromVariant(val))
            {
                dataObj = pyObjectToGtObject(val);
            }
        }

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

    //    GtCommand cmmd = gtApp->startCommand(
    //                gtApp->currentProject(), prop->objectName() +
    //                         QStringLiteral(" of ") + obj->objectName() +
    //                         QStringLiteral(" changed!"));

    bool success = subProp->setValueFromVariant(val, QString());

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

bool
GtpyDecorator::isActive(GtAbstractProperty* prop) const
{
    if (prop == Q_NULLPTR)
    {
        return false;
    }

    return prop->isActive();
}

void
GtpyDecorator::setActive(GtAbstractProperty* prop, bool val)
{
    if (prop == Q_NULLPTR)
    {
        return;
    }

    return prop->setActive(val);
}

bool
GtpyDecorator::isOptional(GtAbstractProperty* prop) const
{
    if (prop == Q_NULLPTR)
    {
        return false;
    }

    return prop->isOptional();
}

void
GtpyDecorator::setOptional(GtAbstractProperty* prop, bool val)
{
    if (prop == Q_NULLPTR)
    {
        return;
    }

    return prop->setOptional(val);
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

#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
double
GtpyDecorator::value(GtDataZone0D* dataZone, const QString& paramName,
                     bool* ok)
{
    if (!dataZone)
    {
        if (ok)
        {
            *ok = false;
        }

        return 0.0;
    }
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    return dataZone->fetchData().value(paramName, ok);
#else
    return dataZone->value(paramName, ok);
#endif
}

QMap<QString, double>
GtpyDecorator::entries(GtDataZone0D* dataZone, bool* ok)
{
    if (!dataZone)
    {
        if (ok)
        {
            *ok = false;
        }

        return QMap<QString, double>();
    }

    QMap<QString, double> retVal;

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    auto data = dataZone->fetchData();
#endif

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    for (QString const& s : data.params())
    {
        retVal.insert(s, data.value(s));
    }
#else
    foreach(QString s, dataZone->params())
    {
        retVal.insert(s, dataZone->value(s));
    }
#endif

    return retVal;
}

bool
GtpyDecorator::setValue(GtDataZone0D* dataZone, const QString& paramName,
                        const double& value)
{
    if (!dataZone)
    {
        return false;
    }

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    auto data = dataZone->fetchData();
    return data.setValue(paramName, value);
#else
    return dataZone->setValue(paramName, value);
#endif
}

bool
GtpyDecorator::appendData(GtDataZone0D* dataZone, const QString& paramName,
                          double value)
{
    if (!dataZone)
    {
        return false;
    }
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    auto data = dataZone->fetchData();
    return data.appendData(paramName, value);
#else
    return dataZone->appendData(paramName, value);
#endif
}

bool
GtpyDecorator::appendData(GtDataZone0D* dataZone, const QString& paramName,
                          const QString& unit, double value)
{
    if (!dataZone)
    {
        return false;
    }
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    auto data = dataZone->fetchData();
    return data.appendData(paramName, unit, value);
#else
    return dataZone->appendData(paramName, unit, value);
#endif
}
#endif
