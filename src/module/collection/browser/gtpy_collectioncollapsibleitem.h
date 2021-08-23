/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectioncollapsibleitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONCOLLAPSIBLEITEM_H
#define GTPYCOLLECTIONCOLLAPSIBLEITEM_H

#include "gtpy_abstractcollectionitem.h"

class GtpyCollectionCollapsibleItem : public GtpyAbstractCollectionItem
{
public:
    GtpyCollectionCollapsibleItem(QString ident);

    virtual bool isCollapsible() const override;

    virtual QString ident() const override;

    virtual void selectAllChildren() override;

    virtual void unselectAllChildren() override;

    virtual QList<GtCollectionNetworkItem> uncollapsibleChilren() override;

    virtual QList<GtCollectionNetworkItem> selectedItems() override;

    GtpyCollectionCollapsibleItem* collapsibleChild(const QString& ident);

    void appendChild(GtpyAbstractCollectionItem* item);

    void appendChild(GtpyAbstractCollectionItem* item, QStringList hierarchy);

private:
    QString m_ident;
};

#endif // GTPYCOLLECTIONCOLLAPSIBLEITEM_H
