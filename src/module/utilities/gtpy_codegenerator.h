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

#include <QObject>

class GtObject;
class GtAbstractProperty;
class GtCalculator;

/**
 * @brief The GtpyCodeGenerator class
 */
class GtpyCodeGenerator : public QObject
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
      * @brief Returns the Python code representing the object path of the
      * given object. The path can be used to access the given object in Python.
      * It starts from the GtPackage that the object belongs to.
      * @param obj The GtObject for which to generate the Python object path.
      * @return The Python code that can be used to access the specified object
      * in Python.
      */
    QString pythonObjectPath(GtObject* obj);

    /**
      * @brief Recursively generates the Python object path for the given object
      * It prepends the object name of the given object to the getterList
      * and calls itself, passing the parent object of the given object. The
      * recursion stops when a GtPackage object is passed and processed, since
      * packages are accessible directly in Python. The recursion also stops if
      * the given object is a nullptr.
      * @param obj The GtObject for which to generate the Python object path.
      * @param getterList A reference to a `QStringList` that stores the path
      * components.
      */
    void generatePythonObjectPath(GtObject* obj, QStringList& getterList);

};

#endif // GTPY_CODEGENERATOR_H
