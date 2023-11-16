/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_decorator.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_DECORATOR_H
#define GTPY_DECORATOR_H

#include <PythonQt.h>

#include <QObject>
#include <QVariant>

#include "gt_globals.h"

class GtCoreApplication;
class GtProject;
class GtTask;
class GtObject;
class GtAbstractProperty;
class GtCalculator;
class GtpyTask;
class GtpyProcessDataDistributor;
class GtProcessComponent;
class GtpyScriptCalculator;

#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
class GtDataZone0D;
#endif

#ifndef Q_MOC_RUN

#define FIND_GT_CHILD
#define FIND_GT_CHILDREN
#define FIND_GT_PROPERTY
#define FIND_GT_PROPERTIES
#define PROPERTY_VALUE
#define SET_PROPERTY_VALUE
#define GET_CHILDREN

#endif

#define FUNCTION_TAG_STR(x) QStringLiteral(#x)

#define FIND_GT_CHILD_TAG FUNCTION_TAG_STR(FIND_GT_CHILD)
#define FIND_GT_CHILDREN_TAG FUNCTION_TAG_STR(FIND_GT_CHILDREN)
#define FIND_GT_PROPERTY_TAG FUNCTION_TAG_STR(FIND_GT_PROPERTY)
#define FIND_GT_PROPERTIES_TAG FUNCTION_TAG_STR(FIND_GT_PROPERTIES)
#define PROPERTY_VALUE_TAG FUNCTION_TAG_STR(PROPERTY_VALUE)
#define SET_PROPERTY_VALUE_TAG FUNCTION_TAG_STR(SET_PROPERTY_VALUE)
#define GET_CHILDREN_TAG FUNCTION_TAG_STR(GET_CHILDREN)

/**
 * @brief The GtPythonDecorators class
 * makes C++ functions callable for python scripts.
 */
class GtpyDecorator: public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent Parent object.
     */
    explicit GtpyDecorator(QObject* parent = NULL);

    /**
     * @brief getFunctionName returns the name of function with given tag.
     * @param funcTag Tag of a function
     * @return Name of function with given tag.
     */
    QString getFunctionName(const QString& funcTag);

    /**
     * @brief Converts a Python object to a GtObject.
     * @param obj Object to convert.
     * @return The converted GtObject.
     */
    static GtObject* pyObjectToGtObject(PythonQtObjectPtr obj);

    static PyObject* wrapGtObject(GtObject* obj);

public slots:

    ///-------> functions of GtCoreApplication <-------\\\

    /**
     * @brief init calls the init() function of GtCoreApplication
     * @param app pointer to GtCoreApplication
     */
    void init(GtCoreApplication* app, const QString& id = QString());

    /**
     * @brief initLanguage calls the initLanguage() function of
     * GtCoreApplication
     * @param app pointer to GtCoreApplication
     */
    void initLanguage(GtCoreApplication* app);

    /**
     * @brief initDatamodel calls the initDatamodel() function of
     * GtCoreApplication
     * @param app pointer to GtCoreApplication
     */
    void initDatamodel(GtCoreApplication* app);

    /**
     * @brief loadModules calls the loadModules() function of GtCoreApplication
     * @param app pointer to GtCoreApplication
     */
    void loadModules(GtCoreApplication* app);

    /**
     * @brief initCalculators calls the initCalculators() function of
     * GtCoreApplication
     * @param app pointer to GtCoreApplication
     */
    void initCalculators(GtCoreApplication* app);

    /**
     * @brief initSession calls the initSession() function of
     * GtCoreApplication
     * @param app pointer to GtCoreApplication
     * @param id of session
     */
    void initSession(GtCoreApplication* app, const QString& id = QString());

    /**
     * @brief switchSession calls the switchSession() of tCoreApplication
     * @param app pointer to tCoreApplication
     * @param id of session
     */
    void switchSession(GtCoreApplication* app, const QString& id);

    /**
     * @brief openProject opens the project with the given projectId
     * @param app pointer to GtCoreApplication
     * @param projectId id of the project that should be opened
     * @return the opened project
     */
    PyObject* openProject(GtCoreApplication* app, const QString& projectId);

    /**
     * @brief currentProject returns the current project
     * @param app pointer to GtCoreApplication
     * @return the current project
     */
    PyObject* currentProject(GtCoreApplication* app);

    ///-------> functions of GtProject <-------\\\

    /**
     * @brief path returns path to project directory on hard drive
     * @param pro pointer to GtProject
     * @return path to project directory
     */
    const QString path(GtProject* pro);

    /**
     * @brief isValid returns whether project data is successfully loaded or not
     * @param pro pointer to GtProject
     * @return validation indicator
     */
    bool isValid(GtProject* pro);

    /**
     * @brief isOpen returns whether project is open
     * @param pro pointer to GtProject
     * @return is open indicator
     */
    bool isOpen(GtProject* pro);

    /**
     * @brief runProcess starts the process with the given id
     * @param pro pointer to GtProject
     * @param processId id of process that should be started
     * @param save indicats whether results should be saved or not
     * @return true if the process was executed successfully
     */
    bool runProcess(GtProject* pro, const QString& processId,
                    bool save = false);

    /**
     * @brief findProcess returns process with the given id
     * @param pro pointer to GtProject
     * @param processId id of process that should be returned
     * @return the process with the given id
     */
    PyObject* findProcess(GtProject* pro, const QString& processId);

    /**
     * @brief close close the project
     * @param pro pointer to GtProject
     * @param save indicats whether results should be saved or not
     * @return true if closing was successful
     */
    bool close(GtProject* pro, bool save = false);

    ///-------> functions of GtCalculator <-------\\\

    /**
     * @brief run calls the exec() function of the given calculator
     * @param calc pointer to GtCalculator
     * @return true if the calculator was executed successfully
     */
    bool run(GtCalculator* calc);

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    /**
     * @brief Decorates inputArgs() method of GtpyScriptCalculator.
     * @param calc Pointer to GtpyScriptCalculator
     * @return The input arguments of the given component as Python dict.
     */
    PyObject* inputArgs(GtpyScriptCalculator* calc);

    /**
     * @brief Decorates inputArg() method of GtpyScriptCalculator.
     * @param calc Pointer to GtpyScriptCalculator
     * @param argName Name of the argument whose value should be returned
     * @return Value of the argument with the given name. If there is no
     * argument with the given name, a std::runtime_error is thrown.
     */
    QVariant inputArg(GtpyScriptCalculator* calc, const QString& argName) const;

    /**
     * @brief Decorates setInputArg() method of GtpyScriptCalculator.
     * @param calc Pointer to GtpyScriptCalculator
     * @param argName Name of the argument whose value should be set
     * @param value The new value of the argument with the given name.
     */
    void setInputArg(GtpyScriptCalculator* calc, const QString& argName,
                     const QVariant& value);

    /**
     * @brief Decorates outputArgs() method of GtpyScriptCalculator.
     * @param calc Pointer to GtpyScriptCalculator
     * @return The output arguments of the given component as Python dict.
     */
    PyObject* outputArgs(GtpyScriptCalculator* calc);

    /**
     * @brief Decorates outputArg() method of GtpyScriptCalculator.
     * @param calc Pointer to GtpyScriptCalculator
     * @param argName Name of the argument whose value should be returned
     * @return Value of the argument with the given name. If there is no
     * argument with the given name, a std::runtime_error is thrown.
     */
    QVariant outputArg(GtpyScriptCalculator* calc, const QString& argName) const;
#endif

    ///-------> functions of GtTask <-------\\\

    /**
     * @brief run calls the exec() function of the given task
     * @param task pointer to GtTask
     * @return true if the task was executed successfully
     */
    bool run(GtTask* task);

    /**
     * @brief Deletes all calculator appended to the given task.
     * @param task pointer to GtTask
     */
    void deleteAllCalculators(GtTask* task);

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    /**
     * @brief Decorates inputArgs() method of GtpyTask.
     * @param task Pointer to GtpyTask
     * @return The input arguments of the given component as Python dict.
     */
    PyObject* inputArgs(GtpyTask* task);

    /**
     * @brief Decorates inputArg() method of GtpyTask.
     * @param task Pointer to GtpyTask
     * @param argName Name of the argument whose value should be returned
     * @return Value of the argument with the given name. If there is no
     * argument with the given name, a std::runtime_error is thrown.
     */
    QVariant inputArg(GtpyTask* task, const QString& argName) const;

    /**
     * @brief Decorates setInputArg() method of GtpyTask.
     * @param task Pointer to GtpyTask
     * @param argName Name of the argument whose value should be set
     * @param value The new value of the argument with the given name.
     */
    void setInputArg(GtpyTask* task, const QString& argName,
                     const QVariant& value);

    /**
     * @brief Decorates outputArgs() method of GtpyTask.
     * @param task Pointer to GtpyTask
     * @return The output arguments of the given component as Python dict.
     */
    PyObject* outputArgs(GtpyTask* task);

    /**
     * @brief Decorates outputArg() method of GtpyTask.
     * @param task Pointer to GtpyTask
     * @param argName Name of the argument whose value should be returned
     * @return Value of the argument with the given name. If there is no
     * argument with the given name, a std::runtime_error is thrown.
     */
    QVariant outputArg(GtpyTask* task, const QString& argName) const;
#endif

    ///-------> functions of GtProcessComponent <-------\\\

    /**
     * @brief Returns state of warning flag.
     * @param comp pointer to GtProcessComponent
     * @return Warning flag.
     */
    bool hasWarnings(GtProcessComponent* comp);

    ///-------> functions of GtpyProcessDataDistributor <-------\\\

    PythonQtPassOwnershipToPython<GtpyProcessDataDistributor*>
    new_GtpyProcessDataDistributor(GtpyTask* pythonTask);

    void delete_GtpyProcessDataDistributor(GtpyProcessDataDistributor* obj);

    PyObject* taskElement(GtpyProcessDataDistributor* obj, const QString& name);

    ///-------> functions of GtObject <-------\\\

    /**
     * @brief findGtChild returns the child of type GtObject with the given
     * child name
     * @param obj pointer to GtObject
     * @param childName name of child that should be found
     * @return See GtObject class documentation.
     */
    FIND_GT_CHILD PyObject* findGtChild(GtObject* obj,
                                        const QString& childName);

    /**
     * @brief findGtChildren returns the children of type GtObject with the
     * given name
     * @param obj pointer to GtObject
     * @param childrenName name of children that should be found
     * @param objectClassName : class name filter
     * @return See GtObject class documentation.
     */
    FIND_GT_CHILDREN QList<PyObject*> findGtChildren(GtObject* obj,
            const QString& childrenName = QString(),
            const QString& objectClassName = QString());

    /**
     * @brief find parent object (GtObject)
     * @param obj : child object
     * @return parent object (GtObject)
     */
    PyObject* findGtParent(GtObject* obj);

    /**
     * @brief findGtProperties returns all properties of a GtObject
     * @param obj pointer to GtObject
     * @return See GtObject class documentation.
     */
    FIND_GT_PROPERTIES QList<GtAbstractProperty*> findGtProperties(
        GtObject* obj);

    /**
     * @brief findGtProperty returns the property with the given id
     * @param obj pointer to GtObject
     * @param id id of property
     * @return See GtObject class documentation.
     */
    FIND_GT_PROPERTY GtAbstractProperty* findGtProperty(GtObject* obj,
            const QString& id);

    /**
     * @brief propertyValue returns the value of the property with given id
     * @param obj pointer to GtObject
     * @param id id of property
     * @return See GtObject class documentation.
     */
    PROPERTY_VALUE QVariant propertyValue(GtObject* obj, const QString& id);

    /**
     * @brief Decorator function to setValueFromVariant function of GtObject.
     * Sets the value of the property with given id to val.
     * @param obj Pointer to GtObject.
     * @param id Id of property.
     * @param See GtObject class documentation.
     */
    SET_PROPERTY_VALUE void setPropertyValue(GtObject* obj, const QString& id,
            QVariant val);

    /**
     * @brief Decorator function to uuid function of GtObject.
     * @param obj Pointer to GtObject.
     * @return See GtObject class documentation.
     */
    QString uuid(GtObject* obj);

    /**
     * @brief returns the object with the given UUID
     * @param obj : root element (searching under this object)
     * @param uuid : univerally unique identifier
     * @return object with the given uuid
     */
    PyObject* objectByUUID(GtObject* obj, const QString& uuid);

    /**
     * @brief Decorator function to calcHash function of GtObject.
     * @param obj Pointer to GtObject.
     * @return See GtObject class documentation.
     */
    QString calcHash(GtObject* obj);

    /**
     * @brief Decorator function to hasChanges function of GtObject.
     * @param obj Pointer to GtObject.
     * @return See GtObject class documentation.
     */
    bool hasChanges(GtObject* obj);

    /**
     * @brief Decorator function to acceptChangesRecursively function of
     * GtObject.
     * @param obj Pointer to GtObject.
     * @return See GtObject class documentation.
     */
    void acceptChangesRecursively(GtObject* obj);


    /**
     * @brief Decorator function to get the class name of a GtObject instance.
     * @param obj Pointer to GtObject.
     * @return Class name of GtObject.
     */
    QString className(GtObject* obj);

    ///-------> functions of GtAbstractProperty <-------\\\

    /**
     * @brief Decorator function to properties function of GtAbstractProperty.
     * @param prop Pointer to GtAbstractProperty.
     * @return See GtAbstractProperty class documentation.
     */
    FIND_GT_PROPERTIES QList<GtAbstractProperty*> findGtProperties(
        GtAbstractProperty* prop);

    /**
     * @brief findGtProperty returns the subproperty with the given id
     * @param prop pointer to GtAbstractProperty
     * @param id id of property
     * @return The subproperty with the given id.
     */
    FIND_GT_PROPERTY GtAbstractProperty* findGtProperty(
        GtAbstractProperty* prop, const QString& id);

    /**
     * @brief Returns the value of the subproperty with given id
     * @param prop pointer to GtAbstractProperty
     * @param id id of property
     * @return Value of the subproperty with given id.
     */
    PROPERTY_VALUE QVariant propertyValue(GtAbstractProperty* prop,
                                          const QString& id);

    /**
     * @brief Sets the value of the property with given id to val
     * @param prop pointer to GtAbstractProperty
     * @param id id of property
     * @param val value that should be set to the property with given id
     */
    SET_PROPERTY_VALUE void setPropertyValue(GtAbstractProperty* prop,
            const QString& id, QVariant val);

    /**
     * @brief Returns identification string of property.
     * @param prop pointer to GtAbstractProperty
     * @return Identification string.
     */
    QString ident(GtAbstractProperty* prop);

    /**
     * @brief Decorator function to isActive function of GtAbstractProperty.
     * @param prop pointer to GtAbstractProperty
     * @return True, if property is active.
     */
    bool isActive(GtAbstractProperty* prop) const;

    /**
     * @brief Decorator function to setActive function of GtAbstractProperty.
     * @param prop pointer to GtAbstractProperty
     * @param val
     */
    void setActive(GtAbstractProperty* prop, bool val);

    /**
     * @brief Decorator function to isOptional function of GtAbstractProperty.
     * @param prop pointer to GtAbstractProperty
     * @return True, if property is optional.
     */
    bool isOptional(GtAbstractProperty* prop) const;

    /**
     * @brief Decorator function to setOptional function of GtAbstractProperty.
     * @param val
     */
    void setOptional(GtAbstractProperty* prop, bool val);

    ///-------> functions of QObject <-------\\\

    /**
     * @brief Returns all children of obj. Primarily used for automatic
     * completion.
     * @param obj Pointer to QObject.
     * @return List of all children of obj.
     */
    GET_CHILDREN QList<QObject*> getChildren(QObject* obj);

    /**
     * @brief Sets the objcet name of obj to name.
     * @param obj Pointer to GtObject.
     * @param name Object name that should be set.
     */
    void setObjectName(QObject* obj, QString name);

    ///-------> functions of GtDataZone0D <-------
#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)
    double value(GtDataZone0D* dataZone, const QString& paramName, bool* ok =
                     Q_NULLPTR);

    /**
     * @brief entries
     * @param dataZone - data to export as map (dictionary in python)
     * @param ok - flag to show success
     * @return a map of the entries of the datazone
     */
    QMap<QString, double> entries(GtDataZone0D* dataZone, bool* ok = Q_NULLPTR);

    bool setValue(GtDataZone0D* dataZone, const QString& paramName,
                  const double& value);

    bool appendData(GtDataZone0D* dataZone, const QString& paramName,
                    double value);

    bool appendData(GtDataZone0D* dataZone, const QString& paramName,
                    const QString& unit, double value);
#endif


signals:
    /**
     * @brief sendPythonConsoleOutput signal for transmitting an output message
     * @param output output message as QString
     */
    void sendErrorMessage(const QString& output) const;
};


#endif // GTPY_DECORATOR_H
