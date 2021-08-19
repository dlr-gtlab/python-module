/* GTlab - Gas Turbine laboratory
 * Source File: gtpyscriptcollectionsettings.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 12.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYSCRIPTCOLLECTIONSETTINGS_H
#define GTPYSCRIPTCOLLECTIONSETTINGS_H

#include "gt_collectionsettings.h"

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
    virtual QWidget* localCollectionWidget(QWidget* parent) Q_DECL_OVERRIDE;

    /**
     * @brief setLocalCollectionData
     * @param editor
     * @param items
     */
    virtual void setLocalCollectionData(QWidget* widget,
                                        QList<GtCollectionItem> items)
    Q_DECL_OVERRIDE;
};

#endif // GTPYSCRIPTCOLLECTIONSETTINGS_H
