/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Stanislaus Reitenbach (AT-TWK)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GTPYTHON_H
#define GTPYTHON_H

#include "gt_moduleinterface.h"
#include "gt_processinterface.h"
#include "gt_mdiinterface.h"
#include "gt_initmoduleinterface.h"
#include "gt_collectioninterface.h"
#include "gt_networkinterface.h"
#include "gt_application.h"

#if GT_VERSION >= 0x010700
#include "gt_versionnumber.h"
#endif

/**
 * @brief The GtPythonModule class
 */
class GtPythonModule: public QObject, public GtModuleInterface,
    public GtProcessInterface, public GtMdiInterface,
    public GtInitModuleInterface, public GtCollectionInterface,
    public GtNetworkInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.dlr.gtlab.GtModuleInterface/0.1"
                      FILE "gt_python.json")

    Q_INTERFACES(GtModuleInterface)
    Q_INTERFACES(GtProcessInterface)
    Q_INTERFACES(GtMdiInterface)
    Q_INTERFACES(GtInitModuleInterface)
    Q_INTERFACES(GtCollectionInterface)
    Q_INTERFACES(GtNetworkInterface)

public:
    /**
     * @brief Returns current version number of datamodel extension
     * @return version number
     */
#if GT_VERSION >= 0x010700
    GtVersionNumber version() Q_DECL_OVERRIDE;
#else
    int version() Q_DECL_OVERRIDE;
#endif

    /**
     * @brief Returns module identification string.
     * @return identification string
     */
    QString ident() const Q_DECL_OVERRIDE;

    /**
     * @brief Returns module description
     * @return description
     */
    QString description() const Q_DECL_OVERRIDE;

    /**
     * @brief Initializes module. Called on application startup.
     */
    void init() Q_DECL_OVERRIDE;

    /**
     * @brief Returns static meta objects of calculator classes.
     * @return list including meta objects
     */
    QList<GtCalculatorData> calculators() Q_DECL_OVERRIDE;

    /**
     * @brief Returns static meta objects of task classes.
     * @return list including meta objects
     */
    QList<GtTaskData> tasks() Q_DECL_OVERRIDE;

    /**
     * @brief Returns static meta objects of mdi item classes.
     * @return list including meta objects
     */
    QList<QMetaObject> mdiItems() Q_DECL_OVERRIDE;

    /**
     * @brief Returns static meta objects of dockwidget classes.
     * @return list including meta objects
     */
    QList<QMetaObject> dockWidgets() Q_DECL_OVERRIDE;

    /**
     * @brief uiItems
     * @return
     */
    QMap<const char*, QMetaObject> uiItems() Q_DECL_OVERRIDE;

    /**
     * @brief postItems
     * @return
     */
    virtual QList<QMetaObject> postItems() Q_DECL_OVERRIDE;

    /**
     * @brief postPlots
     * @return
     */
    QList<QMetaObject> postPlots() Q_DECL_OVERRIDE;

    /**
     * @brief Returns collection specific icon.
     * @return Collection spezific icon.
     */
    QIcon collectionIcon() const Q_DECL_OVERRIDE;

    /**
     * @brief Returns identification string of collection. Same id
     * is used to generate access point data within the netowrk interface.
     * @return Identification string of collection.
     */
    QString collectionId() const Q_DECL_OVERRIDE;

    /**
     * @brief Returns meta object of GtAbstractCollectionSettings class.
     * @return Meta object of GtAbstractCollectionSettings class.
     */
    QMetaObject collectionSettings() const Q_DECL_OVERRIDE;

    /**
     * @brief Returns structure of collection items.
     * @return Sturcture of collection items.
     */
    QMap<QString, QMetaType::Type> collectionStructure() const Q_DECL_OVERRIDE;

    /**
     * @brief Returns identification string of access point.
     * @return Identification string of access point.
     */
    QString accessId() Q_DECL_OVERRIDE;

    /**
     * @brief Returns meta object of GtAbstractAccessDataConnection class.
     * @return Meta object of GtAbstractAccessDataConnection class.
     */
    QMetaObject accessConnection() Q_DECL_OVERRIDE;

private:
    /**
     * @brief Searches recursively for widget based on given objectName path.
     * If parent is not set, current top level widgets of application
     * are analysed.
     * @param path Hierarchical path of objectName strings.
     * @return Widget found based on given objectName path. returns NULL if
     * widget was not found.
     */
    QWidget* findWidget(QStringList path, QWidget* parent = Q_NULLPTR);

};

#endif // GTPYTHON_H
