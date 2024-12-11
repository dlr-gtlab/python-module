/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_decorator.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
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
#if GT_VERSION >= 0x020000
#include "gt_structproperty.h"
#include "gt_propertystructcontainer.h"
#endif
#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
#include "gt_datazone0d.h"
#endif

#include "gtpy_processdatadistributor.h"
#include "gtpy_extendedwrapper.h"
#include "gtpy_task.h"
#include "gtpy_scriptcalculator.h"
#include "gtpy_convert.h"
#include "gtpy_threadscope.h"

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
        return nullptr;
    }

    GtObject* gtObj = nullptr;

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

PyPPObject
GtpyDecorator::wrapGtObject(GtObject* obj, Ownership owner)
{
    if (!obj)
    {
        return {};
    }

    auto pyQtWrapper = PyPPObject::fromQObject(obj);

    if (pyQtWrapper && pyQtWrapper->ob_type->tp_base !=
            &PythonQtInstanceWrapper_Type)
    {
        return {};
    }

    auto childArg = PyPPTuple_New(1);

    PyPPTuple_SetItem(childArg, 0, std::move(pyQtWrapper));

    auto retval = PyPPObject::NewRef(GtpyExtendedWrapper_Type.tp_new(
        &GtpyExtendedWrapper_Type, childArg.get(), nullptr));

    GtpyExtendedWrapper* self = (GtpyExtendedWrapper*)(retval.get());

    if (owner == Python || owner == ForcePython)
    {
        if (self->_obj) self->_obj->passOwnershipToPython();
    }
    if (owner == ForcePython)
    {
        self->forcePythonOwnership = true;
    }

    return retval;

}

PyPPObject
GtpyDecorator::wrapGtObject(std::unique_ptr<GtObject>&& obj)
{
    if (!obj)
    {
        return {};
    }

    auto pyObj = wrapGtObject(obj.get(), Python);
    // Python is now the owner, remove it from CPP
    if (pyObj) obj.release();

    return pyObj;
}

void
GtpyDecorator::init(GtCoreApplication* app, const QString& id)
{
    if (app == nullptr)
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
    if (app == nullptr)
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
    if (app == nullptr)
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
    if (app == nullptr)
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
    if (app == nullptr)
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
    if (app == nullptr)
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
    if (app == nullptr)
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

PyObjectAPIReturn GtpyDecorator::openProject(GtCoreApplication* app,
                                     const QString& projectId)
{
    if (app == nullptr)
    {
        QString output = QStringLiteral("ERROR: ") +
                         QObject::tr("helper is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return nullptr;
    }

    qDebug() << "open project...     projectId == " << projectId;

    if (projectId.isEmpty())
    {
        QString output = QStringLiteral("ERROR: ") +
                         QObject::tr("project id is empty!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return nullptr;
    }

    GtProject* project = app->findProject(projectId);

    if (project == nullptr)
    {
        QString output = QStringLiteral("ERROR: ") +
                         QObject::tr("project not found!") +
                         QStringLiteral(" (") + projectId + QStringLiteral(")");

        qWarning() << output;

        emit sendErrorMessage(output);

        return nullptr;
    }

    if (!gtDataModel->GtCoreDatamodel::openProject(project))
    {
        QString output = QStringLiteral("ERROR: ") +
                         QObject::tr("could not open project!") +
                         QStringLiteral(" (") + projectId +
                         QStringLiteral(")");

        qWarning() << output;

        emit sendErrorMessage(output);

        return nullptr;
    }

    qDebug() << "project opened!";

    return wrapGtObject(project).release();
}

PyObjectAPIReturn
GtpyDecorator::currentProject(GtCoreApplication* app)
{
    if (app == nullptr)
    {
        QString output = QStringLiteral("ERROR: ") +
                         QObject::tr("app is a null pointer!");

        qWarning() << output;

        emit sendErrorMessage(output);

        return nullptr;
    }

    return wrapGtObject(app->currentProject()).release();
}

const QString
GtpyDecorator::path(GtProject* pro)
{
    if (pro == nullptr)
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
    if (pro == nullptr)
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
    if (pro == nullptr)
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
    if (pro == nullptr)
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

    if (process == nullptr)
    {
        QString output =  QStringLiteral("ERROR: ") +
                          QObject::tr("process not found!") +
                          QStringLiteral(" (") + processId +
                          QStringLiteral(")");

        qWarning() << output;

        emit sendErrorMessage(output);

        return false;
    }

    {
        // The executor runs python tasks in a different thread, so we need to call this first
        auto _ = GtpyThreadScope();

        // execute process
        GtCoreProcessExecutor executor;
        executor.runTask(process);
    }

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

PyObjectAPIReturn
GtpyDecorator::findProcess(GtProject* pro, const QString& processId)
{
    if (pro == nullptr || processId.isEmpty())
    {
        return nullptr;
    }

    return wrapGtObject(pro->findProcess(processId)).release();
}

bool
GtpyDecorator::run(GtCalculator* calc)
{
    bool success = false;

    if (calc != nullptr)
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
PyObjectAPIReturn
GtpyDecorator::inputArgs(GtpyScriptCalculator* calc) const
{
    return gtpy::convert::fromPropertyStructContainer(calc->inputArgs()).release();
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

PyObjectAPIReturn
GtpyDecorator::outputArgs(GtpyScriptCalculator* calc) const
{
    return gtpy::convert::fromPropertyStructContainer(calc->outputArgs()).release();
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

    if (task != nullptr)
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
//    if (process == nullptr)
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
    if (pro == nullptr)
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
    if (task == nullptr)
    {
        return;
    }

    Py_BEGIN_ALLOW_THREADS

    QList<GtProcessComponent*> calcs = task->findDirectChildren <
                                       GtProcessComponent* > ();

    int lastIndex = calcs.size() - 1;

    for (; lastIndex >= 0; lastIndex--)
    {
        GtProcessComponent* calc = calcs.at(lastIndex);
        calcs.removeAt(lastIndex);
        delete calc;
    }

    Py_END_ALLOW_THREADS
}

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
PyObjectAPIReturn
GtpyDecorator::inputArgs(GtpyTask* task) const
{
    return gtpy::convert::fromPropertyStructContainer(task->inputArgs()).release();
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

PyObjectAPIReturn
GtpyDecorator::outputArgs(GtpyTask* task) const
{
    return gtpy::convert::fromPropertyStructContainer(task->outputArgs()).release();
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

    auto childs = comp->findChildren<GtProcessComponent*>();

    return std::any_of(std::begin(childs), std::end(childs), [](auto* child){
        return child->currentState() == GtProcessComponent::WARN_FINISHED ||
               child->currentState() == GtProcessComponent::FAILED;
    });
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

PyObjectAPIReturn
GtpyDecorator::taskElement(
    GtpyProcessDataDistributor* obj, const QString& name)
{
    GtTask* task = obj->taskElement(name);

    return wrapGtObject(task).release();
}

PyObjectAPIReturn
GtpyDecorator::findGtChild(GtObject* obj, const QString& childName)
{
    if (obj == nullptr || childName.isEmpty())
    {
        return nullptr;
    }

    GtObject* child = obj->findDirectChild<GtObject*>(childName);
    return wrapGtObject(child).release();
}

QList<PyObjectAPIReturn>
GtpyDecorator::findGtChildren(GtObject* obj, const QString& childrenName,
                              const QString& objectClassName)
{
    if (obj == nullptr)
    {
        return QList<PyObjectAPIReturn>();
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

    QList<PyObjectAPIReturn> retval;

    foreach (GtObject* child, children)
    {
        retval.append(wrapGtObject(child).release());
    }

    return retval;
}

QList<PyObjectAPIReturn>
GtpyDecorator::findGtChildrenByClass(GtObject* obj,
                                     const QString& objectClassName)
{
    return findGtChildren(obj, QString(), objectClassName);
}

PyObjectAPIReturn
GtpyDecorator::findGtParent(GtObject* obj)
{
    if (!obj)
    {
        gtError() << __func__ << " -> Invalid object given!";

        return nullptr;
    }

    return wrapGtObject(obj->findParent<GtObject*>()).release();
}
#if GT_VERSION >= 0x020000

int
GtpyDecorator::getPropertyContainerSize(GtObject* obj, const QString& id)
{
    if (!obj)
    {
        gtError() << __func__ << " -> Invalid object given!";
        return -1;
    }

    GtPropertyStructContainer* s = obj->findPropertyContainer(id);

    if (!s)
    {
        gtError() << __func__ << " -> PropertyStruct container of "
                                 "object not fund!";
        return -1;
    }

    return s->size();
}

QVariant
GtpyDecorator::getPropertyContainerVal(GtObject* obj, QString const& id,
                                       int index, QString const& memberId)
{
    if (!obj)
    {
        gtError() << __func__ << " -> Invalid object given!";
        return {};
    }

    GtPropertyStructContainer* s = obj->findPropertyContainer(id);

    if (!s)
    {
        gtError() << __func__ << " -> PropertyStruct container of "
                                 "object not fund!";
        return {};
    }

    if (index < 0 || index >= s->size())
    {
        gtError() << __func__ << " -> Index is invalid for "
                                 "requested property container!";
        return {};
    }

    GtPropertyStructInstance& structCon = s->at(index);

    return structCon.getMemberValToVariant(memberId);
}

bool
GtpyDecorator::setPropertyContainerVal(GtObject* obj, const QString& id,
                                       int index, const QString& memberId,
                                       const QVariant& val)
{
    if (!obj)
    {
        gtError() << __func__ << " -> Invalid object given!";
        return false;
    }

    GtPropertyStructContainer* s = obj->findPropertyContainer(id);

    if (!s)
    {
        gtError() << __func__ << " -> PropertyStruct container of "
                                 "object not fund!";
        return false;
    }

    if (index < 0 || index >= s->size())
    {
        gtError() << __func__ << " -> Index is invalid for "
                                 "requested property container!";
        return {};
    }


    GtPropertyStructInstance& structCon = s->at(index);

    return structCon.setMemberVal(memberId, val);
}
#endif
QList<GtAbstractProperty*>
GtpyDecorator::findGtProperties(GtObject* obj)
{
    if (!obj) return {};

    return obj->fullPropertyList();
}

GtAbstractProperty*
GtpyDecorator::findGtProperty(GtObject* obj, const QString& id)
{
    if (obj == nullptr || id.isEmpty())
    {
        return nullptr;
    }

    return obj->findProperty(id);
}

QVariant
GtpyDecorator::propertyValue(GtObject* obj, const QString& id)
{
    if (obj == nullptr || id.isEmpty())
    {
        return QVariant();
    }

    GtAbstractProperty* prop = obj->findProperty(id);

    if (prop == nullptr)
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
    if (obj == nullptr || id.isEmpty())
    {
        return;
    }

    GtAbstractProperty* prop = obj->findProperty(id);

    if (prop == nullptr)
    {
        QString message = "\n\n" + QString(obj->metaObject()->className()) +
                          " has no GtProperty with the id: " + id +
                          "\nIf you want to set a QProperty, use the function "
                          "setProperty()!";

        throw std::runtime_error(message.toStdString());

        return;
    }

    if (auto* objLinkProp = qobject_cast<GtObjectLinkProperty*>(prop))
    {
        auto* dataObj = qvariant_cast<GtObject*>(val);

        if (!dataObj)
        {
            if (PythonQtObjectPtr().fromVariant(val))
            {
                dataObj = pyObjectToGtObject(val);
            }
        }

        if (dataObj)
        {
            QString objClassName = QString::fromUtf8(
                                       dataObj->metaObject()->className());

#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
            QStringList allowedClasses = objLinkProp->allowedClasses();
            if (allowedClasses.contains(objClassName))
            {
                val = dataObj->uuid();
            }
#else
            if (objLinkProp->isAllowed(objClassName))
            {
                val = dataObj->uuid();
            }
#endif

            else
            {
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
                QStringList allowedClasses = objLinkProp->allowedClasses();
#endif

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

PyObjectAPIReturn
GtpyDecorator::objectByUUID(GtObject* obj, const QString& uuid)
{
    Q_UNUSED(obj)

    return wrapGtObject(obj->getObjectByUuid(uuid)).release();
}

PyObjectAPIReturn
GtpyDecorator::clone(GtObject *obj)
{
    if (!obj) return nullptr;

    // the clone is a new object, we transfer ownership to python
    std::unique_ptr<GtObject> cloned_obj(obj->clone());
    return wrapGtObject(std::move(cloned_obj)).release();
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
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->metaObject()->className();
}

QList<GtAbstractProperty*>
GtpyDecorator::findGtProperties(GtAbstractProperty* prop)
{
    if (prop == nullptr)
    {
        return QList<GtAbstractProperty*>();
    }

    return prop->properties();
}

GtAbstractProperty*
GtpyDecorator::findGtProperty(GtAbstractProperty* prop, const QString& id)
{
    if (prop == nullptr || id.isEmpty())
    {
        return nullptr;
    }

    return prop->findProperty(id);
}

QVariant
GtpyDecorator::propertyValue(GtAbstractProperty* prop, const QString& id)
{
    if (prop == nullptr || id.isEmpty())
    {
        return QVariant();
    }

    GtAbstractProperty* subProp = prop->findProperty(id);

    if (subProp == nullptr)
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
    if (prop == nullptr || id.isEmpty())
    {
        return;
    }

    GtAbstractProperty* subProp = prop->findProperty(id);

    if (subProp == nullptr)
    {
        QString message = "\n\n" + QString(prop->metaObject()->className()) +
                          " has no subproperty of type GtProperty with the "
                          "id: " + id;

        throw std::runtime_error(message.toStdString());

        return;
    }

    if (auto* objLinkProp = qobject_cast<GtObjectLinkProperty*>(subProp))
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
            QString objClassName = QString::fromUtf8(
                                       dataObj->metaObject()->className());


#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
            QStringList allowedClasses = objLinkProp->allowedClasses();
            if (allowedClasses.contains(objClassName))
            {
                val = dataObj->uuid();
            }
#else
            if (objLinkProp->isAllowed(objClassName))
            {
                val = dataObj->uuid();
            }
#endif

            else
            {
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
                QStringList allowedClasses = objLinkProp->allowedClasses();
#endif

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
    if (prop == nullptr)
    {
        return QString();
    }

    return prop->ident();
}

bool
GtpyDecorator::isActive(GtAbstractProperty* prop) const
{
    if (prop == nullptr)
    {
        return false;
    }

    return prop->isActive();
}

void
GtpyDecorator::setActive(GtAbstractProperty* prop, bool val)
{
    if (prop == nullptr)
    {
        return;
    }

    return prop->setActive(val);
}

bool
GtpyDecorator::isOptional(GtAbstractProperty* prop) const
{
    if (prop == nullptr)
    {
        return false;
    }

    return prop->isOptional();
}

void
GtpyDecorator::setOptional(GtAbstractProperty* prop, bool val)
{
    if (prop == nullptr)
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
    if (obj == nullptr)
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
