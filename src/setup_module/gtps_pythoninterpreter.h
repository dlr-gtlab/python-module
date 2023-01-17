/* GTlab - Gas Turbine laboratory
 * Source File: gtps_pythoninterpreter.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 20.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPSPYTHONEVALUATOR_H
#define GTPSPYTHONEVALUATOR_H

#include <QString>
#include <QStringList>
#include <QFile>

#include "gt_versionnumber.h"

/**
 * @brief The GtpsPythonEvaluator class
 */
class GtpsPythonInterpreter
{
public:
    /**
     * @brief GtpsPythonEvaluator
     * @param pythonExe Path to the Python executable.
     */
    explicit GtpsPythonInterpreter(const QString& pythonExe);

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
    const QString& pythonExe() const;

    /**
     * @brief Returns the Python version the evaluator uses.
     * @return The Python version the evaluator uses.
     */
    const GtVersionNumber& version() const;

    /**
     * @brief Returns the shared Python library filename
     *        (e.g. /path/to/libpython.3.9.so)
     * @return Filename of to the shared Python library.
     */
    const QString& sharedLib() const;

    /**
     * @brief Returns the list of paths that are registered in sys.path.
     * @return The list of paths that are registered in sys.path.
     */
    const QStringList& sysPaths() const;

    /**
     * @brief Retruns the PYTHONHOME path.
     * @return The PYTHONHOME path.
     */
    QString pythonHomePath() const;

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

    /**
     * @brief Runs a python script
     * @param scriptFile The file to the script. Note, this file needs to
     *        be already written to disk and must have a filename.
     *
     * @param ok Optional control flag. Is true if the evaluation
     * was successful.
     * @return The output string resulting from the execution of the
     * Python code.
     */
    QString runScript(const QFile& scriptFile, bool* ok = nullptr) const;

    /**
     * @brief Function to call the python interpreter with generic arguments
     * @param args The argument list to pass to python
     * @param ok Optional control flag. Is true if the evaluation
     * was successful.
     * @return The output string resulting from the execution of the
     * Python code.
     */
    QString runPythonInterpreter(const QStringList& args, bool* ok = nullptr) const;


private:
    // Python sys paths.
    QStringList m_sysPaths{};

    // Python executalbe.
    QString m_pythonExe{};

    // Shared python library
    QString m_sharedLib{};

    // Python version.
    GtVersionNumber m_pyVersion{};

    /**
     * @brief Asks the Python interpreter for the Python version and stores
     * the result in the version variable.
     */
    void setPythonVersion();

    /**
     * @brief Asks the interpreter for the path to the shared library and
     * stores it in the shared lib variable.
     */
    void setSharedLibPath();

    /**
     * @brief Asks the interpreter for the paths registered in sys.path and
     * stores them in the sys paths variable.
     */
    void setSysPaths();
};

#endif // GTPSPYTHONEVALUATOR_H
