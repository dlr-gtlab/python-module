/* GTlab - Gas Turbine laboratory
 * Source File: ResourcesModule
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 04.11.2025
 * Author: Marvin Nöthen (DLR AT-TWK)
 */

#ifndef MODULE_H
#define MODULE_H

// #include "gt_moduleinterface.h"
#include "gt_datamodelinterface.h"
#include "gt_mdiinterface.h"
#include "gt_importerinterface.h"

namespace gt
{

namespace resource
{

/**
 * @generated 1.3.0
 * @brief The gt::resource::Module class
 */
class Module : public QObject, /*public GtModuleInterface,*/
               public GtDatamodelInterface, public GtMdiInterface,
               public GtImporterInterface
{
    Q_OBJECT

    // GT_MODULE("gt_resourcesmodule.json")

    // Q_INTERFACES(GtModuleInterface)
    Q_INTERFACES(GtDatamodelInterface)
    Q_INTERFACES(GtMdiInterface)
    Q_INTERFACES(GtImporterInterface)

public:

    // /**
    //  * @brief Returns current version number of module
    //  * @return version number
    //  */
    // GtVersionNumber version() override;

    // /**
    //  * @brief Returns module description
    //  * @return description
    //  */
    // QString description() const override;

    // /**
    //  * @brief Initializes module. Called on application startup.
    //  */
    // void init() override;

    // /**
    //  * @brief Passes additional module information to the framework.
    //  *
    //  * NOTE: A reference to the author can significantly help the user to
    //  * know who to contact in case of issues or other request.
    //  * @return module meta information.
    //  */
    // MetaInformation metaInformation() const override;

    /**
     * @brief Returns static meta objects of datamodel package.
     * @return package meta object
     */
    QMetaObject package() override;

    /**
     * @brief Returns static meta objects of datamodel classes.
     * @return list including meta objects
     */
    QList<QMetaObject> data() override;

    /**
     * @brief Returns true if module is a standalone module with own data
     * model structure. Otherwise module only extends the overall application
     * with additional functionalities like classes, calculators or
     * graphical user interfaces.
     * @return Standalone indicator.
     */
    bool standAlone() override;

    /**
     * @brief Returns static meta objects of mdi item classes.
     * @return list including meta objects
     */
    QList<QMetaObject> mdiItems() override;

    /**
     * @brief Returns static meta objects of dockwidget classes.
     * @return list including meta objects
     */
    QList<QMetaObject> dockWidgets() override;

    /**
     * @brief uiItems
     * @return data class names mapped to ui item objects
     */
    QMap<const char*, QMetaObject> uiItems() override;

    /**
     * @brief postItems
     * @return
     */
    QList<QMetaObject> postItems() override;

    /**
     * @brief postPlots
     * @return
     */
    QList<QMetaObject> postPlots() override;

    /**
     * @brief Returns static meta objects of importer classes.
     * @return List of meta objects of exporter classes.
     */
    QList<QMetaObject> importer() const override;
};

} // namespace resource

} // namespace gt

#endif // MODULE_H
