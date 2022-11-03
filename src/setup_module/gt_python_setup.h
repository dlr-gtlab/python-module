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

#if GT_VERSION >= 0x010700
#include "gt_versionnumber.h"
#endif

/**
 * @brief The GtPythonModule class
 */
class GtPythonSetupModule: public QObject, public GtModuleInterface
#if GT_VERSION < 0x020000
    , public GtInitModuleInterface
#endif
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.dlr.gtlab.GtModuleInterface/0.1"
                      FILE "gt_python_setup.json")

    Q_INTERFACES(GtModuleInterface)

#if GT_VERSION < 0x020000
    Q_INTERFACES(GtInitModuleInterface)
#endif

    GT_MODULE

public:
    /**
     * @brief Returns current version number of datamodel extension
     * @return version number
     */
#if GT_VERSION >= 0x010700
    GtVersionNumber version() override;
#else
    int version() Q_DECL_OVERRIDE;
#endif

    /**
     * @brief Returns module description
     * @return description
     */
    QString description() const override;

    void onLoad() override;
};

#endif // GTPYTHON_H
