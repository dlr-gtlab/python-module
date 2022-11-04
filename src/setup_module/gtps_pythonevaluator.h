/* GTlab - Gas Turbine laboratory
 * Source File: gtps_pythonevaluator.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 20.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPSPYTHONEVALUATOR_H
#define GTPSPYTHONEVALUATOR_H

#include <QString>

#include "gt_versionnumber.h"

/**
 * @brief The GtpsPythonEvaluator class
 */
class GtpsPythonEvaluator
{
public:
    /**
     * @brief GtpsPythonEvaluator
     * @param pythonExe Path to the Python executable.
     */
    explicit GtpsPythonEvaluator(const QString& pythonExe);

    /**
     * @brief Returns true if the Python executalbe is valid and the evaluator
     * can execute Python code. Otherwise it returns false.
     * @return True, if the evaluator can execute Python code.
     */
    bool isValid() const;

    /**
     * @brief Returns the path to the Python executable.
     * @return The path to the Python executalbe.
     */
    QString pythonExe() const;

    /**
     * @brief Returns the Python version the evaluator uses.
     * @return The Python version the evaluator uses.
     */
    GtVersionNumber pythonVersion() const;

    /**
     * @brief Evaluates the given pythonCommand using the Python executable
     * passed to the constructor.
     * @param pythonCommand Python command to evaluate.
     * @param ok Optional control flag. Is true if the evaluation
     * was successful.
     * @return The output string resulting from the execution of the
     * Python code.
     */
    QString eval(const QString& pythonCommand, bool* ok = nullptr) const;

private:
    // Python executalbe.
    QString m_pythonExe{};

    // Python version.
    GtVersionNumber m_pyVersion{};

    /**
     * @brief Asks the Python execution for the Python version and stores
     * the result in the version variable.
     */
    void setPythonVersion();
};

#endif // GTPSPYTHONEVALUATOR_H
