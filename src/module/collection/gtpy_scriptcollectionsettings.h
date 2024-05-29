/* GTlab - Gas Turbine laboratory
 * Source File: gtpyscriptcollectionsettings.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2021
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPYSCRIPTCOLLECTIONSETTINGS_H
#define GTPYSCRIPTCOLLECTIONSETTINGS_H

#include "gt_collectionsettings.h"
#include "gt_globals.h"

/**
 * @brief The GtpyScriptCollectionSettings class
 */
class GtpyScriptCollectionSettings : public GtCollectionSettings
{
    Q_OBJECT

public:
    /**
     * @brief GtpyScriptCollectionSettings
     */
    Q_INVOKABLE GtpyScriptCollectionSettings();

    /**
     * @brief localCollectionWidget
     * @param parent
     */
    virtual QWidget* localCollectionWidget(QWidget* parent) override;

    /**
     * @brief setLocalCollectionData
     * @param editor
     * @param items
     */
    virtual void setLocalCollectionData(QWidget* widget,
                                        QList<GtCollectionItem> items) override;

    /**
     * @brief browseCollectionWidget
     * @param parent
     */
    virtual GtAbstractBrowserWidget* browserWidget(QWidget* parent) override;

    /**
     * @brief setBrowserWidgetData
     * @param widget
     * @param installedItems
     * @param availableItems
     * @param updataAvailableItems
     */
#if GT_VERSION >= 0x020000
    virtual void setBrowserWidgetData(
        GtAbstractBrowserWidget* widget,
        const QList<GtCollectionNetworkItem>& installedItems,
        const QList<GtCollectionNetworkItem>& availableItems,
        const QList<GtCollectionNetworkItem>& updataAvailableItems) override;
#else
    virtual void setBrowserWidgetData(
        GtAbstractBrowserWidget* widget,
        QList<GtCollectionNetworkItem> installedItems,
        QList<GtCollectionNetworkItem> availableItems,
        QList<GtCollectionNetworkItem> updataAvailableItems) override;
#endif



};

#endif // GTPYSCRIPTCOLLECTIONSETTINGS_H
