/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_decorator.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_DECORATOR_H
#define GTPY_DECORATOR_H

#include <PythonQt.h>

#include <QObject>
#include <QVariant>

#include <memory>

#include <gt_globals.h>
#include "gtpypp.h"
#include "gtpy_globals.h"
#include "gt_pythonmodule_exports.h"

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
class GT_PYTHON_EXPORT GtpyDecorator: public QObject
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

    enum Ownership
    {
        CPP,         // The object's lifetime is managed by C++
        Python,      // The object's lifetime is managed by Python
        ForcePython  // The object should be managed by Python, even though the object is part of a C++ tree
    };

    /**
     * @brief Creates a python objecz from a GtObject
     *
     * Note, this function assumes, that the ownership remains at the C++ side,
     * except the ownership argument is changed
     *
     * @param obj bject to be returned to python
     * @param forcePythonOwnership
     * @return
     */
    static PyPPObject wrapGtObject(GtObject* obj, Ownership owner = CPP);

    /**
     * @brief Creates a Python object from obj and transfers ownership to
     *        Python.
     *
     * @param obj Object to be returned to python
     * @return
     */
    static PyPPObject wrapGtObject(std::unique_ptr<GtObject>&& obj);

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
     * @param projectIdOrPath Id or the path of the project that should be opened
     * @return the opened project
     */
    PyObjectAPIReturn openProject(GtCoreApplication* app, const QString& projectIdOrPath);


    /**
     * @brief currentProject returns the current project
     * @param app pointer to GtCoreApplication
     * @return the current project
     */
    PyObjectAPIReturn currentProject(GtCoreApplication* app);

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
    PyObjectAPIReturn findProcess(GtProject* pro, const QString& processId);

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
    PyObjectAPIReturn inputArgs(GtpyScriptCalculator* calc) const;

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
    PyObjectAPIReturn outputArgs(GtpyScriptCalculator* calc) const;

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
    PyObjectAPIReturn inputArgs(GtpyTask* task) const;

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
    PyObjectAPIReturn outputArgs(GtpyTask* task) const;

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

    PyObjectAPIReturn taskElement(GtpyProcessDataDistributor* obj, const QString& name);

    ///-------> functions of GtObject <-------\\\

    /**
     * @brief findGtChild returns the child of type GtObject with the given
     * child name
     * @param obj pointer to GtObject
     * @param childName name of child that should be found
     * @return See GtObject class documentation.
     */
    FIND_GT_CHILD PyObjectAPIReturn findGtChild(GtObject* obj,
                                        const QString& childName);

    /**
     * @brief findGtChildren returns the children of type GtObject with the
     * given name
     * @param obj pointer to GtObject
     * @param childrenName name of children that should be found
     * @param objectClassName : class name filter
     * @return See GtObject class documentation.
     */
    FIND_GT_CHILDREN QList<PyObjectAPIReturn> findGtChildren(GtObject* obj,
            const QString& childrenName = QString(),
            const QString& objectClassName = QString());

    /**
     * @brief findGtChildrenByClass returns the children of type GtObject
     * of the given class name
     * @param obj pointer to GtObject
     * @param objectClassName : class name filter
     * @return See GtObject class documentation.
     */
    QList<PyObjectAPIReturn> findGtChildrenByClass(GtObject* obj,
            const QString& objectClassName);

    /**
     * @brief find parent object (GtObject)
     * @param obj : child object
     * @return parent object (GtObject)
     */
    PyObjectAPIReturn findGtParent(GtObject* obj);

#if GT_VERSION >= 0x020000
    /**
     * @brief getPropertyContainerSize
     * Find a struct container property and return a value of
     * a defined propety
     * @param obj - parent object of the struct propety
     * @param id of the property struct container
     * @return size of the container (number of elements in the container)
     * Returns -1 for invalid inputs
     */
    int getPropertyContainerSize(GtObject* obj, const QString& id);

    /**
     * @brief getPropertyContainerVal
     * Find a struct container property and return a value of
     * a defined propety
     * @param obj - parent object of the struct propety
     * @param id of the property struct container
     * @param index of the element of the items of the container
     * @param memberId of the property to call
     * @return the value as a variant - empty for invalid inputs
     *
     * Example call in python:
     *  task.getPropertyContainerVal("args_input", 0, "name")
     */
    QVariant getPropertyContainerVal (GtObject* obj, const QString& id,
                                      int index, const QString& memberId);

    /**
     * Same as above, but uses the ident of the property entry to find it
     */

    /**
     * @brief getPropertyContainerVal
     * Find a struct container property and return a value of
     * a defined propety
     * @param obj - parent object of the struct propety
     * @param id of the property struct container
     * @param entryId - id of the entry to read the value from.
     * In contrast to function above this function does not use the index
     * @param memberId of the property to call
     * @return the value as a variant - empty for invalid inputs
     *
     * Example call in python:
     *  task.getPropertyContainerVal("constraints", "minEta", "value")
     */
    QVariant getPropertyContainerVal(GtObject* obj, const QString& id,
                                     const QString& entryId,
                                     const QString& memberId);

    /**
     * @brief getPropertyContainerEntryIds
     * Returns a list with all property entry ids of a given container element
     * The container element is specified by its containerId
     * @param obj - object which has the property container
     * @param containerId - Id of the property container to evaluate
     * @return a ist of all ids of the contained properties
     */
    QStringList getPropertyContainerEntryIds(GtObject* obj,
                                             const QString& containerId);

    /**
     * @brief setPropertyContainerVal
     * Find a struct container property and sets a value of
     * a defined propety
     * @param obj - parent object of the struct propety
     * @param id of the property struct container
     * @param index of the element of the items of the container
     * @param memberId of the property to call
     * @param val - value to set
     * @return true in case of success, else false
     */
    bool setPropertyContainerVal(GtObject* obj, QString const& id, int index,
                                 QString const& memberId, const QVariant& val);

    /**
     * @brief setPropertyContainerVal
     * Find a struct container property and sets a value of
     * a defined propety
     * @param obj - parent object of the struct propety
     * @param id of the property struct container
     * @param entryId - Id of the container element to modify. Incontrast to the
     * function above the elment is determined by id and not by index
     * @param memberId of the property to call
     * @param val - value to set
     * @return true in case of success, else false
     */
    bool setPropertyContainerVal(GtObject* obj, QString const& id,
                                 const QString& entryId,
                                 QString const& memberId, const QVariant& val);
#endif
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
    PyObjectAPIReturn objectByUUID(GtObject* obj, const QString& uuid);

    /**
     * @brief Clones the given object
     *
     * A new object is returned, the ownership is transferred to python.
     *
     * @param obj The object to be cloned
     * @return    A new object
     */
    PyObjectAPIReturn clone(GtObject* obj);

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
                     nullptr);

    /**
     * @brief entries
     * @param dataZone - data to export as map (dictionary in python)
     * @param ok - flag to show success
     * @return a map of the entries of the datazone
     */
    QMap<QString, double> entries(GtDataZone0D* dataZone, bool* ok = nullptr);

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
    void sendErrorMessage(const QString& output);
};


#endif // GTPY_DECORATOR_H
