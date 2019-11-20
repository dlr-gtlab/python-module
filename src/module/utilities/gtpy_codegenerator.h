/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_codegenerator.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 18.11.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_CODEGENERATOR_H
#define GTPY_CODEGENERATOR_H

#include <QObject>

#include "gt_objectmemento.h"

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
     * given memento. Only the changes are inserted into the script.
     * @param calc Edit calculator object.
     * @param before Memento object of the calculator before editing.
     * @return Python code representing the calculator settings.
     */
    QString calculatorPyCode(GtCalculator* calc, GtObjectMemento before);

protected:
    /**
     * @brief The GtpyCodeGenerator constructor.
     * @param parent Parent object.
     */
    GtpyCodeGenerator(QObject* parent = Q_NULLPTR);

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
};

#endif // GTPY_CODEGENERATOR_H
