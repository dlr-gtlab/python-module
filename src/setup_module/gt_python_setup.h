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
#include "gt_application.h"

#include "gt_versionnumber.h"

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
    /**
     * @brief Asks the specified evaluator for the Python paths and prepends
     * them to the PATH environment variable. In addition, it sets the
     * PYTHONHOME variable.
     * @param interpreter Python interpreter.
     */
    void setPythonPaths(const GtpsPythonInterpreter& interpreter);
};

#endif // GTPYTHON_H
