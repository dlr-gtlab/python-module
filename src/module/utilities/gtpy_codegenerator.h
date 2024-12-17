/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_codegenerator.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 18.11.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_CODEGENERATOR_H
#define GTPY_CODEGENERATOR_H

#include "gt_pythonmodule_exports.h"

#include <QObject>

class GtObject;
class GtAbstractProperty;
class GtCalculator;

/**
 * @brief The GtpyCodeGenerator class
 */
class GT_PYTHON_EXPORT GtpyCodeGenerator : public QObject
{
    Q_OBJECT

public:    
    /**
     * @brief Returns instance of code generator based on singleton pattern.
     * @return Instance of code generator
     */
    static GtpyCodeGenerator* instance();

    /**
     * @brief Creates the Python code representing the calculator settings and
     * returns it. The the setting of the claculator will be compared with the
     * default settings of the calculator. Only the changes are inserted into
     * the script.
     * @param calc Edit calculator object.
     * @return Python code representing the calculator settings.
     */
    QString calculatorPyCode(GtCalculator* calc);

protected:
    /**
     * @brief The GtpyCodeGenerator constructor.
     * @param parent Parent object.
     */
    explicit GtpyCodeGenerator(QObject* parent = nullptr);

private:
    /**
     * @brief Converts the property value of the given property to python value
     * and returns it as string.
     * @param prop Property whose value should be converted.
     * @return Value of property as python string
     */
    QString propValToString(GtAbstractProperty* prop);

    /**
     * @brief Generates the python code to initialize a helper object.
     * @param obj Helper object
     * @param pyObjName Name of object into python environment.
     * @return The generated python code as string value.
     */
    QString helperPyCode(GtObject* obj, const QString& pyObjName);

    /**
      * @brief Returns the object path of the specified object as Python code.
      * The object path can be used to access the object in Python. It begins
      * with the GtPackage to which the object belongs.
      * @param obj The GtObject for which to generate the Python object path.
      * @return The object path of the specified object which can be used to
      * access the corresponding object in Python.
      */
    QString pythonObjectPath(GtObject* obj) const;

    /**
      * @brief Recursively generates the object path of the specified object.
      * It prepends the Python getter method of the given object to the
      * getterList and calls itself, passing the parent object of the given
      * object. The recursion stops when a GtPackage object is passed and
      * processed or when the given object is a nullptr.
      *
      * To use the object path in Python, join the entries of the getterList
      * with '.'.
      *
      * @param obj The GtObject for which to generate the Python object path.
      * @param getterList A reference to a QStringList that stores the getter
      * methods representing the object path of the specified object in Python.
      */
    void generatePythonObjectPath(GtObject* obj, QStringList& getterList) const;

};

#endif // GTPY_CODEGENERATOR_H
