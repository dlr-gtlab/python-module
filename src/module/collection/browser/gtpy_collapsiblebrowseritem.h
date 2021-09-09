/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collapsiblebrowseritem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONCOLLAPSIBLEITEM_H
#define GTPYCOLLECTIONCOLLAPSIBLEITEM_H

#include "gtpy_abstractbrowseritem.h"

class GtpyCollapsibleBrowserItem : public GtpyAbstractBrowserItem
{
public:
    GtpyCollapsibleBrowserItem(QString ident);

    virtual bool isCollapsible() const override;

    virtual QString ident() const override;

    virtual void selectAllChildren() override;

    virtual void unselectAllChildren() override;

    virtual QList<GtCollectionNetworkItem> uncollapsibleChilren() override;

    virtual QList<GtCollectionNetworkItem> selectedItems() override;

    void appendChild(GtpyAbstractBrowserItem* item, QStringList hierarchy);

private:
    void appendChild(GtpyAbstractBrowserItem* item);

    GtpyCollapsibleBrowserItem* collapsibleChild(const QString& ident);

    QString m_ident;
};

#endif // GTPYCOLLECTIONCOLLAPSIBLEITEM_H
