/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsermodelitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONITEM_H
#define GTPYCOLLECTIONITEM_H

#include "gt_collectionnetworkitem.h"

#include "gtpy_abstractcollectionitem.h"

class GtpyCollectionItem : public GtpyAbstractCollectionItem
{
public:
    GtpyCollectionItem(const GtCollectionNetworkItem& item);

    virtual ~GtpyCollectionItem();

    virtual bool isCollapsible() const override;

    virtual QString ident() const override;

    virtual QString version() const override;

    virtual QString installedVersion() const override;

    virtual GtCollectionNetworkItem item() const override;
private:
    /// Collection network item.
    GtCollectionNetworkItem m_item;
};

#endif // GTPYCOLLECTIONITEM_H
