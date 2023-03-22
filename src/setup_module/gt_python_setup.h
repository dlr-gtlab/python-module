/* GTlab - Gas Turbine laboratory
 * Source File: gt_python_setup.h
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.10.2022
 *  Author: Marvin Nöthen (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GTPYTHON_H
#define GTPYTHON_H

#include "gt_moduleinterface.h"

class GtpsPythonInterpreter;

/**
 * @brief The GtPythonModule class
 */
class GtPythonSetupModule: public QObject, public GtModuleInterface
{
    Q_OBJECT
    GT_MODULE("gt_python_setup.json")

    Q_INTERFACES(GtModuleInterface)

public:
    /**
     * @brief Returns current version number of datamodel extension
     * @return version number
     */
    GtVersionNumber version() override;

    /**
     * @brief Returns module description
     * @return description
     */
    QString description() const override;

    /**
     * @brief Is called directly after loading the module.
     * It validates the Python environment and suppresses the Python Module
     * if the environment is not valid.
     */
    void onLoad() override;

    /**
     * @brief To register the python preferences dialog
     */
    void init() override;

private:
    struct ErrorMsg
    {
        QString error;
        QString task;
    };

    /// Error message
    ErrorMsg m_errorMsg;

    /**
     * @brief It prepends the Python paths to the PATH environment variable.
     * In addition, it sets the PYTHONHOME and the PYTHONPATH variable.
     * @param interpreter Python interpreter.
     */
    void setPythonPaths(const GtpsPythonInterpreter& interpreter);

    /**
     * @brief Opens a message box to ask the user to
     * specify a Python interpreter.
     * @param msg Message for the user.
     */
    void showPythonErrorNotification(const ErrorMsg& error);

    /**
     * @brief Accepts the Python Module built with the specified Python
     * version. That means it suppresses all supported Python Modules except
     * the module built with the specified version.
     * @param Python version to be accepted.
     */
    void acceptPythonModule(const GtVersionNumber& version);

    /**
     * @brief Suppresses all Python Modules built with the Python versions
     * specified in the vector of versions.
     * @param Vector of Python versions to be suppressed.
     */
    void suppressPythonModules(const QVector<GtVersionNumber>& pyVersions);
};

#endif // GTPYTHON_H
