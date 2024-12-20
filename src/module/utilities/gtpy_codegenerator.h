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
     * @brief Generates the python code to initialize a helper object.
     * @param obj Helper object
     * @param pyObjName Name of object into python environment.
     * @return The generated python code as string value.
     */
    QString helperPyCode(GtObject* obj, const QString& pyObjName);

};

#endif // GTPY_CODEGENERATOR_H
